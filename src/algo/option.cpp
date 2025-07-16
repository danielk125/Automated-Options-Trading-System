#include "../../include/option.h"
#include <iostream>

using std::cout;

Option::Option(
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
    ) : 
    _optionType(optionType),
    _assetPrice(assetPrice),
    _year(year),
    _month(month),
    _day(day),
    _strikePrice(strikePrice), 
    _bid(bid), 
    _ask(ask), 
    _bidSize(bidSize), 
    _askSize(askSize), 
    _volume(volume), 
    _quoteDetail(quoteDetail),
    _greeks(Greeks(rho, vega, theta, delta, gamma, iv, curValue))
{
    
}

void Option::printOption() {
    cout << (_optionType ? "PUT OPTION, " : "CALL OPTION, ")
        << "Expiration Date: " << _month << ' ' << _day << ' ' << _year << ", "
        << "Strike Price: " << _strikePrice << '\n';
}

double Option::priceOption() {
    
}
