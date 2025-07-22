#pragma once

#include "util.h"

#include <string>

using std::string;

struct Greeks {
    double rho;
    double vega;
    double theta;
    double delta;
    double gamma;

    double iv;
    bool currentValue;

    Greeks(double r, double v, double t, double d, double g, double i, bool curVal) : 
    rho(r), vega(v), theta(t), delta(d), gamma(g), iv(i), currentValue(curVal)
    {}
};

class Option {
    OptionType _optionType;
    double _assetPrice;
    int _year;
    int _month;
    int _day;
    double _strikePrice;
    double _bid;
    double _ask;
    int _bidSize;
    int _askSize;
    int _volume;
    string _quoteDetail;
    Greeks _greeks;

public:
    Option(
        OptionType optionType, 
        double assetPrice,
        int year,
        int month,
        int day,
        double strikePrice, 
        double bid, 
        double ask, 
        int bidSize, 
        int askSize, 
        int volume, 
        double rho, 
        double vega, 
        double theta, 
        double delta, 
        double gamma, 
        double iv, 
        bool curValue,
        string quoteDetail
    );

    void printOption();

    double getprice();

    friend class Algo;
};
