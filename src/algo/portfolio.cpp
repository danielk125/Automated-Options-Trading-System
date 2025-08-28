#include "../../include/portfolio.hpp"

Portfolio::Portfolio(OptionMap& omap) : _OMap(omap) {
    loadClosingFilter();
};

void Portfolio::loadPortfolio() {
    if (std::filesystem::exists("portfolio.json")) {
        auto j = json::parse(std::ifstream("portfolio.json"));
        cash = j["Balance"]["cashAvailableForInvestment"];
        value = j["Balance"]["RealTimeValues"]["totalAccountValue"];

        for (auto& position : j["Positions"]) {
            Position pos;

            pos.entryPrice =    position["pricePaid"];
            pos.size =          position["quantity"];
            pos.curValue =      position["marketValue"];
            pos.posSide =       position["positionType"] == "LONG" ? LONG : SHORT;
            pos.secType =       position["Product"]["securityType"] == "OPTN" ? OPTN : EQ;

            string symbol =     position["Product"]["symbol"];
            
            if (pos.secType == OPTN) {
                int day =           position["Product"]["expiryDay"];
                int month =         position["Product"]["expiryMonth"];
                int year =          position["Product"]["expiryYear"];
                string expDate = std::to_string(month) + '_' + std::to_string(day) + '_' + std::to_string(year);

                double strike =     position["Product"]["strikePrice"];
                OptionType type =   position["Product"]["callPut"] == "CALL" ? CALL : PUT;

                pos.secInfo = std::make_unique<OptionAbreviator>(symbol, expDate, strike, type);

            } else if (pos.secType == EQ) {
                pos.secInfo = std::make_unique<Abreviator>(symbol);
            }

            _currentPositions.push_back(std::move(pos));
        }
    }
}

void Portfolio::loadClosingFilter(){
    string filename = "filters/close_filter.csv";

    std::ifstream file(filename);

    string rawline;
    std::getline(file, rawline);
    string field;
    std::stringstream line(rawline);
    vector<string> fields;

    while(std::getline(line, field, ','))
        fields.push_back(field);
    
    bool includeAutoSell                = std::stoi(fields[0]);
    int numWeeksFromExpiration          = std::stoi(fields[1]);
    bool includeClosingThreshold        = std::stoi(fields[2]);
    double mispriceClosingThreshold     = std::stod(fields[3]);
    bool includeStopLossPercent         = std::stoi(fields[4]);
    double stopLossPercent              = std::stod(fields[5]);
    bool includeStopLossAmount          = std::stoi(fields[6]);
    double stopLossAmount               = std::stod(fields[7]);

    _filter = ClosingFilter(includeAutoSell, 
                            numWeeksFromExpiration, 
                            includeClosingThreshold, 
                            mispriceClosingThreshold, 
                            includeStopLossPercent, 
                            stopLossPercent, 
                            includeStopLossAmount,
                            stopLossAmount);
}

vector<Order> Portfolio::closePositions() {
    vector<Order> res;

    for (auto& pos : _currentPositions){
        if (pos.secType == EQ) continue;
        
        auto* oa = dynamic_cast<OptionAbreviator*>(pos.secInfo.get());

        if (oa->symbol != _OMap._symbol)
            continue;

        const OptionPair op = _OMap[oa->expDate][oa->strikePrice];
        Option option = (oa->type == CALL) ? op.getCall() : op.getPut();

        double mpt = _OMap.accessMispriceThreshold();
        double misprice;
        Order order;
        if (pos.posSide == LONG) {
            misprice = option.getMispriceBuy();
            order.ordSide = SELL;
            order.price = option.getBid();
        } else {
            misprice = option.getMispriceSell();
            order.ordSide = BUY;
            order.price = option.getAsk();
        }

        if (_filter.markPosition(pos.posSide, pos.entryPrice, option)){
            order.secType = OPTN;
            order.size = 1;
            order.secInfo = std::make_unique<OptionAbreviator>(_OMap._symbol, option.getExpDate(), option.getStrike(), option.getOptionType());

            if (order.ordSide == SELL) {
                cash += option.getBid() * 100;
            } else {
                cash -= option.getAsk() * 100;
            }

            res.push_back(std::move(order));
        }
    }

    return res;
}

vector<Order> Portfolio::openPositions() {
    vector<Order> res;
    
    while (auto contract = _OMap.getFilteredContract()){
        Option option = *contract;

        if (option.checkBuy()){
            if (option.getAsk() * 100 > cash) 
                continue;

            Order order;
            order.ordSide = BUY;
            order.secType = OPTN;
            order.price = option.getAsk();
            order.size = 1;
            order.secInfo = std::make_unique<OptionAbreviator>(_OMap._symbol, option.getExpDate(), option.getStrike(), option.getOptionType());

            cash -= option.getAsk() * 100;

            res.push_back(std::move(order));
        } else {
            Order order;
            order.ordSide = SELL;
            order.secType = OPTN;
            order.price = option.getBid();
            order.size = 1;
            order.secInfo = std::make_unique<OptionAbreviator>(_OMap._symbol, option.getExpDate(), option.getStrike(), option.getOptionType());

            cash += option.getBid() * 100;

            res.push_back(std::move(order));
        }
    }

    return res;
}

void Portfolio::executeOrders() {
    vector<Order> positionsToClose = closePositions();
    vector<Order> positionsToOpen = openPositions();

    std::filesystem::path folder = "orders";
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directories(folder);
    }

    std::filesystem::path filename = folder / (_OMap._symbol + "_orders.csv");

    std::ofstream file(filename);
    file << "SIDE TO CLOSE, SYMBOL, EXP DATE, STRIKE, CALL/PUT, PRICE, SIZE\n";

    file << "--POSITIONS TO CLOSE--\n";
    for (auto& o : positionsToClose) {
        auto* oa = dynamic_cast<OptionAbreviator*>(o.secInfo.get());
        file << ((o.ordSide == BUY) ? "BUY," : "SELL,");
        file << oa->symbol << ',';
        file << oa->expDate << ',';
        file << std::to_string(oa->strikePrice) << ',';
        file << ((oa->type == CALL) ? "CALL," : "PUT,");
        file << std::to_string(o.price) << ',';
        file << std::to_string(o.size) << '\n';
    }

    file << "--POSITIONS TO OPEN--\n";
    for (auto& o : positionsToOpen) {
        auto* oa = dynamic_cast<OptionAbreviator*>(o.secInfo.get());
        file << ((o.ordSide == BUY) ? "BUY," : "SELL,");
        file << oa->symbol << ',';
        file << oa->expDate << ',';
        file << std::to_string(oa->strikePrice) << ',';
        file << ((oa->type == CALL) ? "CALL," : "PUT,");
        file << std::to_string(o.price) << ',';
        file << std::to_string(o.size) << '\n';
    }

    file.close();
}

double Portfolio::calculateCurrentValue() const{
    double value {};
    for (const auto& pos : _currentPositions){
        value += pos.curValue;
    }
    return value * 100;
}

double Portfolio::closePositionsBacktest(double& gain){
    double cash_change {0};
    double gain_temp {0};
    std::erase_if(_currentPositions,
        [this, &cash_change, &gain_temp] (const Position& pos) {
            auto* oa = dynamic_cast<OptionAbreviator*>(pos.secInfo.get());
            const OptionPair op = _OMap[oa->expDate][oa->strikePrice];
            Option option = (oa->type == CALL) ? op.getCall() : op.getPut();

            bool close = _filter.markPosition(pos.posSide, pos.entryPrice, option);

            if (close) {
                if (pos.posSide == LONG) {
                    cash_change += (100 * pos.curValue);
                    gain_temp += 100 * (pos.curValue - pos.entryPrice);
                } 
                else                        cash_change -= (100 * pos.curValue);
            }

            return close;
        }
    );
    gain = gain_temp;
    return cash_change;
}

void Portfolio::updatePositionsBacktest(){
    for (auto& pos : _currentPositions){
        auto* oa = dynamic_cast<OptionAbreviator*>(pos.secInfo.get());
        const OptionPair op = _OMap[oa->expDate][oa->strikePrice];
        Option option = (oa->type == CALL) ? op.getCall() : op.getPut();

        pos.curValue = option.getBid();
    }
}

double Portfolio::openPositionsBacktest(){
    double cur_cash {cash};
    while (auto contract = _OMap.getFilteredContract()){
        Option option = *contract;
        Position pos;

        if (option.checkBuy()){
            if (option.getAsk() * 100 > cur_cash) 
                continue;

            pos.posSide = LONG;
            pos.secType = OPTN;
            pos.entryPrice = option.getAsk();
            pos.curValue = option.getBid();
            pos.size = 1;
            pos.secInfo = std::make_unique<OptionAbreviator>(_OMap._symbol, option.getExpDate(), option.getStrike(), option.getOptionType());

            cur_cash -= option.getAsk() * 100 * pos.size;

            _currentPositions.push_back(std::move(pos));
        } else {
            pos.posSide = SHORT;
            pos.secType = OPTN;
            pos.entryPrice = option.getBid();
            pos.curValue = option.getAsk();
            pos.size = 1;
            pos.secInfo = std::make_unique<OptionAbreviator>(_OMap._symbol, option.getExpDate(), option.getStrike(), option.getOptionType());

            cur_cash += option.getBid() * 100 * pos.size;

            _currentPositions.push_back(std::move(pos));
        }
    }
    return cash - cur_cash;
}

double Portfolio::calculateUnrealizedGain(){
    double gain {0};
    for (auto& pos : _currentPositions){
        gain += (pos.curValue - pos.entryPrice) * 100;
    }
    return gain;
}

void Portfolio::loadPortfolioBacktest(){
    if (!std::filesystem::exists("portfolio_backtest.json")) {
        cash = 10000.0;
        value = 0.0;
        unrealizedGain = 0.0;
        realizedGain = 0.0;
        _currentPositions.clear();
        return;
    }

    std::ifstream in("portfolio_backtest.json");
    json j = json::parse(in);

    cash           = j.value("cash", 0.0);
    value          = j.value("value", 0.0);
    unrealizedGain = j.value("unrealizedGain", 0.0);
    realizedGain   = j.value("realizedGain", 0.0);   

    _currentPositions.clear();

    const auto& jpos = j.at("positions");

    for (const auto& jp : jpos) {
        Position p{};
        p.posSide    = (jp.at("posSide").get<std::string>() == "LONG") ? LONG : SHORT;
        p.secType    = (jp.at("secType").get<std::string>() == "OPTN") ? OPTN : EQ;
        p.entryPrice = jp.value("entryPrice", 0.0);
        p.curValue   = jp.value("curValue", 0.0);
        p.size       = jp.value("size", 0);

        if (p.secType == OPTN) {
            string oa_symbol        = jp.value("symbol", std::string{});
            string oa_expDate       = jp.value("expDate", std::string{});
            double oa_strikePrice   = jp.value("strikePrice", 0.0);
            OptionType oa_type      = (jp.value("type", "CALL") == "CALL") ? CALL : PUT;

            p.secInfo = std::make_unique<OptionAbreviator>(oa_symbol, oa_expDate, oa_strikePrice, oa_type);
        } else {
            p.secInfo = nullptr;
        }

        _currentPositions.push_back(std::move(p));
    }
}

void Portfolio::savePortfolioBacktest(){
    double newRealizedGain {realizedGain};
    double cash_change_closing = closePositionsBacktest(newRealizedGain);
    cash += cash_change_closing;
    updatePositionsBacktest();
    double cash_change_opening = openPositionsBacktest();

    double cur_cash = cash - cash_change_opening;
    double curValue = calculateCurrentValue() + cur_cash;
    double newUnrealizedGain = calculateUnrealizedGain();
    

    json j;
    j["cash"] = cur_cash;
    j["value"] = curValue;
    j["unrealizedGain"] = newUnrealizedGain;
    j["realizedGain"] = newRealizedGain;
    j["positions"] = json::array();
    
    for (const auto& pos : _currentPositions) {
        json jp;
        jp["posSide"]    = (pos.posSide == LONG ? "LONG" : "SHORT");
        jp["secType"]    = (pos.secType == OPTN ? "OPTN" : "EQ");
        jp["entryPrice"] = pos.entryPrice;
        jp["curValue"]   = pos.curValue;
        jp["size"]       = pos.size;

        if (auto* oa = dynamic_cast<OptionAbreviator*>(pos.secInfo.get())) {
            jp["symbol"]      = oa->symbol;
            jp["expDate"]     = oa->expDate;
            jp["strikePrice"] = oa->strikePrice;
            jp["type"]        = (oa->type == CALL ? "CALL" : "PUT");
        }

        j["positions"].push_back(jp);
    }

    std::ofstream out("portfolio_backtest.json");
    out << j.dump(4);
}
