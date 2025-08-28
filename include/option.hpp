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
        double iv,
        string startDate = ""
    );

    //write overload for backtesting

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
    bool checkBuy() const;
    bool checkSell() const;

    void printOption() const;

    void markBuy(bool b);
    void markSell(bool s);

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

    int _day;
    int _month;
    int _year;

    bool _buy;
    bool _sell;

    void calculateTimeToExpiration(string startDate = "");

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

class OptionPair {
    Option _callOption;
    Option _putOption;

public:
    OptionPair(){
        _callOption = Option();
        _putOption = Option();
    }

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

    void setCall(Option o) {
        _callOption = o;
    }

    void setPut(Option o) {
        _putOption = o;
    }
};