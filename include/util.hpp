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

enum OrderType {
    MARKET,
    LIMIT
};

class OptionPair {
    Option _callOption;
    Option _putOption;

public:
    OptionPair(Option call, Option put) : _callOption(call), _putOption(put) {
        if (call.getOptionType() != CALL || put.getOptionType() != PUT){
            throw std::invalid_argument("OptionPair must be (CALL, PUT)");
        }
    }

    const Option& getCall() const {
        return _callOption;
    }

    const Option& getPut() const {
        return _putOption;
    }
};

struct OptionAbrv {
    string symbol;
    string expDate;
    double strikePrice;
    OptionType type;
};