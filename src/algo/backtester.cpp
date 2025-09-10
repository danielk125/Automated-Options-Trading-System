#include "../../include/backtester.hpp"

Backtester::Backtester(string symbol, bool customFilter) : _symbol(symbol), _customFilter(customFilter) {}

std::optional<std::string> Backtester::earliestCsvDate(const std::filesystem::path& dir) {
    std::string best;
    for (auto& e : std::filesystem::directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;

        auto name = e.path().filename().string();
        auto pos  = name.find("_backtest.csv");
        if (pos == std::string::npos) continue;

        auto date = name.substr(0, pos);

        if (best.empty() || date < best) best = std::move(date);
    }

    if (best.empty()) return std::nullopt;
    return best;
}

void Backtester::retrieveDatesAndPrices(string startDate){
    string assetFile = "backtest_CSV/" + _symbol + "/asset_backtest.csv";
    if (!std::filesystem::exists(assetFile)) {
        return;
    }

    bool date_flag = false;
    std::ifstream file(assetFile);
    string rawLine;
    bool firstline = true;
    while(std::getline(file, rawLine)){
        if (firstline){
            firstline = false;
            continue;
        }
        string date {""};
        double assetPrice {0.0};
        std::stringstream line(rawLine);
        string temp;
        for (int i = 0; i < 5; ++i){
            std::getline(line, temp, ',');
            if (i == 0){
                date = temp;
            } else if (i == 4) {
                assetPrice = std::stod(temp);
            } 
        }
        
        if (date == startDate){
            _dates.push_back(date);
            dailyAssetPrices[date] = assetPrice;
            break;
        }
        
        _dates.push_back(date);
        dailyAssetPrices[date] = assetPrice;
    }
}

void Backtester::step(string& date){
    OptionMap o(_symbol, _customFilter, date, dailyAssetPrices[date]);
    Portfolio p(o);
    p.loadPortfolioBacktest();
    p.savePortfolioBacktest();
}

void Backtester::run(){
    const std::filesystem::path backtestDir = std::filesystem::path("backtest_CSV") / _symbol;

    auto start = earliestCsvDate(backtestDir);
    if (!start) {
        throw std::runtime_error("No backtest CSVs found in " + backtestDir.string());
    }

    retrieveDatesAndPrices(*start);
    for (auto iter = _dates.rbegin(); iter != _dates.rend(); ++iter){
        string date = *iter;
        const path csv = backtestDir / (date + "_backtest.csv");
        if (std::filesystem::exists(csv)) {
            step(date);
        }
    }
}