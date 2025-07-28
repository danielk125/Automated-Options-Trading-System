#pragma once

#include "util.h"

#include <string>
#include <iostream>
#include <ctime>
#include <vector>

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
    double _eTime;
    double _fairValue;

    void timeToExpiration(int e_year, int e_month, int e_day);

    void calculate_binomial_params(
        int& N,
        double& r,
        double& dt,
        double& u,
        double& d,
        double& p,
        double& df,
        const double& T,
        const double& sigma
    );

    double calculatePayoff(double val, double K, OptionType type);

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

    double price_binomial();

    friend class Algo;
};
