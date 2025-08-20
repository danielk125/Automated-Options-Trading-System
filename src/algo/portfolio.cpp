#include "../../include/portfolio.hpp"
/*
struct Position {
    PositionSide posSide;
    SecurityType secType;
    double entryPrice;
    OptionAbrv contractInfo;
    double curValue;
    int size;
};

struct OptionAbrv {
    string symbol;
    string expDate;
    double strikePrice;
    OptionType type;
};
*/

Portfolio::Portfolio(OptionMap& omap) : _OMap(omap) {};

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

vector<Order> Portfolio::closePositions() {
    vector<Order> res;

    for (auto& pos : _currentPositions){
        if (pos.secType == EQ) continue;
        
        auto* oa = dynamic_cast<OptionAbreviator*>(pos.secInfo.get());

        if (oa->symbol != _OMap._symbol)
            continue;

        string expDate = oa->expDate;
        double strike = oa->strikePrice;

        const OptionPair op = _OMap[expDate][strike];

        Option option;
        if (oa->type == CALL) {
            option = op.getCall();
        } else {
            option = op.getPut();
        }

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

        if (misprice < mpt){
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

            option.printOption();

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

    std::filesystem::path folder = "./orders";
    if (!std::filesystem::exists(folder)) {
        std::filesystem::create_directories(folder);   // creates "orders" and any parents if missing
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
