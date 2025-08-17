#include "../../include/option_filter.hpp"

bool OptionFilter::checkTime(double timeToExpiration){
    const double weekToYear = 7/365;

    double startWeek = numWeeksToExpirationStart * weekToYear;
    double endWeek = numWeeksToExpirationEnd * weekToYear;

    return (timeToExpiration >= startWeek && timeToExpiration <= endWeek);
}

bool OptionFilter::checkMisprice(double misprice){
    return misprice > mispriceThresholdPercent;
}

bool OptionFilter::checkCall(OptionType ot){
    return ot == CALL;
}

bool OptionFilter::checkPut(OptionType ot){
    return ot == PUT;
}

OptionFilter::OptionFilter(){
    numWeeksToExpirationStart = -1;
    numWeeksToExpirationEnd = -1;
    mispriceThresholdPercent = -1;
    includeCalls = -1;
    includePuts = -1;
    includeBuys = -1;
    includeSells = -1;
}

OptionFilter::OptionFilter(int nws, int nwe, double mpth, bool ic, bool ip, bool ib, bool is) : 
    numWeeksToExpirationStart(nws),
    numWeeksToExpirationEnd(nwe),
    mispriceThresholdPercent(mpth),
    includeCalls(ic),
    includePuts(ip),
    includeBuys(ib),
    includeSells(is)
{};

bool OptionFilter::markOption(Option& option){
    bool expCheck = checkTime(option.timeToExpiration());

    bool misPriceBuy = includeBuys && checkMisprice(option.getMispriceBuy());
    option.markBuy(misPriceBuy);
    bool misPriceSell = includeSells && checkMisprice(option.getMispriceSell());
    option.markSell(misPriceSell);
    bool misPriceCheck = misPriceBuy || misPriceSell;

    bool callCheck = includeCalls && checkCall(option.getOptionType());
    bool putCheck = includePuts && checkPut(option.getOptionType());
    bool typeCheck = callCheck || putCheck;

    return expCheck && typeCheck && misPriceCheck;
}

double OptionFilter::getMisprice(){
    return mispriceThresholdPercent;
}