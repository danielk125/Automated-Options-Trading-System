#pragma once

#include "option.h"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using std::unordered_map;
using std::vector;

class OptionChain {
    Asset _a;
    unordered_map<string, vector<Option>> _chain;
    vector<ExpiryDate> _dates;

public:

    OptionChain(string filename);

    OptionChain(string option_filename, double assetPrice, string symbol);
    // constructor overload for parsing backtest data

    unordered_map<string, vector<Option>>* getChain();

    vector<Option> getSingleChain(int i);

    void printChain();

    void printChain(string& dateID);

    string getSymbol();

    Asset getAsset();
};
