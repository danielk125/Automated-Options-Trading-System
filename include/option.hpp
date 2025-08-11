#pragma once

#include "util.hpp"

#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

using std::string;

class Option {
public:
    Option();

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
        int openInterest,
        double rho, 
        double vega, 
        double theta, 
        double delta, 
        double gamma, 
        double iv
    );

    string getExpDate() const;
    double getStrike() const;
    OptionType getOptionType() const;
    double getAssetPrice() const;
    double getBid() const;
    double getAsk() const;
    int getVolume() const;
    int getOI() const;
    int getBidSize() const;
    int getAskSize() const;
    double getIV() const;
    double timeToExpiration() const;
    double fairValue() const;
    double getDelta() const;
    double getGamma() const;
    double getTheta() const;
    double getRho() const;
    double getVega() const;
    double getMispriceBuy() const;
    double getMispriceSell() const;

    void printOption() const;

private:
    string _expirationDate;
    double _strikePrice;
    OptionType _optionType;
    double _assetPrice; 
    double _bid;
    double _ask;
    int _volume;
    int _openInterest;
    int _bidSize;
    int _askSize;
    double _impliedVol;
    double _eTime;
    double _fairValue;
    double _mispricePercentBuy;
    double _mispricePercentSell;
    double _delta;
    double _gamma;
    double _theta;
    double _rho;
    double _vega;

    void calculateTimeToExpiration(int e_year, int e_month, int e_day);

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
    ) const;

    double calculatePayoff(double val, double K, OptionType type) const;

    void calculateMisprice();

    double price_binomial();
};
