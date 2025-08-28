#pragma once

#include "portfolio.hpp"
#include "option_map.hpp"

using std::filesystem::path;

class Backtester {
public:
    Backtester(string symbol, bool customFilter);

    void step(string& date);
    
    void run();

private:
    string _symbol;
    bool _customFilter;
    vector<string> _dates;
    unordered_map<string, double> dailyAssetPrices;

    std::optional<std::string> earliestCsvDate(const std::filesystem::path& dir);
    void retrieveDatesAndPrices(string startDate);
};