#pragma once

#include <string>

using std::string;

enum OptionType {
    CALL,
    PUT
};

struct Asset {
    string symbol;
    double price;
    int volume;

    Asset(string s, double p, int v) : symbol(s), price(p), volume(v) {}
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
