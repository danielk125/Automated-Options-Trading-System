#pragma once

#include <string>

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

struct Asset {
    string symbol;
    double price;
};

struct ExpiryDate {
    int month;
    int year;
    int day;

    string dateID;

    ExpiryDate(int m, int d, int y) : month(m), day(d), year(y){
        dateID  =  std::to_string(m) + '_' +
                    std::to_string(d) + '_' +
                    std::to_string(y); 
    }
};
