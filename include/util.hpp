#pragma once

#include <string>
#include <tuple>

using std::string;

enum OptionType {
    CALL,
    PUT
};

enum OrderSide {
    BUY,
    SELL
};

enum PositionSide {
    LONG,
    SHORT
};

enum SecurityType {
    EQ,
    OPTN
};

enum OrderType {
    MARKET,
    LIMIT
};

struct Abreviator {
    string symbol;
    explicit Abreviator(string s) : symbol(s) {}
    virtual ~Abreviator() = default;
};

struct OptionAbreviator : public Abreviator {
    string expDate;
    double strikePrice;
    OptionType type;

    OptionAbreviator(string s, string exp, double k, OptionType t) : 
        Abreviator(s), expDate(exp), strikePrice(k), type(t) {}
};