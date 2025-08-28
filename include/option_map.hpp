#pragma once

#include "option.hpp"
#include "option_filter.hpp"

#include <unordered_map>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <print>

using std::unordered_map;
using std::vector;

class OptionChain {
public:
    OptionChain(unordered_map<double, OptionPair>& chain) : _chain(chain) {};

    const OptionPair operator[](double K) const { return _chain.at(K); }

private:
    unordered_map<double, OptionPair>& _chain;
};

class OptionMap {
public:
    string _symbol;

    void load_from_Etrade(string symbol);

    void load_for_backtest(string symbol, string date, double assetPrice);

    OptionMap(string symbol, bool customFilter);

    OptionMap(string symbol, bool customFilter, string date, double assetPrice);

    //OptionMap(string option_filename, double assetPrice, string symbol);
    // constructor overload for parsing backtest data

    const unordered_map<double, OptionPair>& getSingleChain(std::string_view) const;

    void printChain() const;

    // overload [] operator to function as unordered map access
    const OptionChain operator[](std::string_view dateID);

    std::optional<Option> getFilteredContract();

    double accessMispriceThreshold();

private:
    void setMapField(string date, double k, OptionPair op);

    unordered_map<string, unordered_map<double, OptionPair>> _map;

    struct cmp {
        bool operator()(const Option& o1, const Option& o2){
            return o1.getMispriceBuy() < o2.getMispriceBuy(); // larger misprice on top
        }
    };

    std::priority_queue<Option, std::vector<Option>, cmp> _filtered;

    OptionFilter _filter;

    void loadFilter(string filter_filename);

};
