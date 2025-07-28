#pragma once

#include "option.h"

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using std::unordered_map;
using std::vector;

class OptionChain {
    string _symbol;
    unordered_map<string, vector<Option>> _chain;

public:

    OptionChain(string filename);

    unordered_map<string, vector<Option>>* getChain();

    void printChain();

    void printChain(string& dateID);

    string getSymbol();
};
