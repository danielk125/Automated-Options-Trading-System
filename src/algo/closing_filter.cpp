#include "../../include/closing_filter.hpp"

ClosingFilter::ClosingFilter() {
    includeAutoSell = 0;
    numWeeksFromExpiration = 0;
    includeClosingThreshold = 0;
    mispriceClosingThreshold = 0;
    includeStopLossPercent = 0;
    stopLossPercent = 0;
    includeStopLossAmount = 0;
    stopLossAmount = 0;
}

ClosingFilter::ClosingFilter(bool ias, int nwe, bool ict, double mpct, bool islp, double slp, bool isla, double sla) {
    includeAutoSell = ias;
    numWeeksFromExpiration = nwe;
    includeClosingThreshold = ict;
    mispriceClosingThreshold = mpct;
    includeStopLossPercent = islp;
    stopLossPercent = slp;
    includeStopLossAmount = isla;
    stopLossAmount = sla;
}

bool ClosingFilter::checkTime(double timeToExpiration){
    const double weekToYear = 7.0/365.0;

    double timeLeft = numWeeksFromExpiration * weekToYear;

    return (timeToExpiration <= timeLeft);
}

bool ClosingFilter::checkMisprice(double misprice){
    return (misprice <= mispriceClosingThreshold);
}

bool ClosingFilter::checkStopLossPercent(double initialCost, double currentvalue){
    if (currentvalue > initialCost) return false;

    double lossPercent = (initialCost - currentvalue) / initialCost;
    return lossPercent >= stopLossPercent;
}

bool ClosingFilter::checkStopLossAmount(double initialCost, double currentvalue){
    if (currentvalue > initialCost) return false;

    double loss = initialCost - currentvalue;
    return loss >= stopLossAmount;
}

bool ClosingFilter::markPosition(PositionSide pside, double entryPrice, Option& option){
    bool shouldClose = false;

    bool timeCheck = includeAutoSell && checkTime(option.timeToExpiration());
    bool mispriceCheckBuy = (pside == SHORT ? true : false) && checkMisprice(option.getMispriceSell());
    bool mispriceCheckSell = (pside == LONG ? true : false) && checkMisprice(option.getMispriceBuy());
    bool mispriceCheck = includeClosingThreshold && (mispriceCheckBuy || mispriceCheckSell);
    bool SLPCheck = includeStopLossPercent && checkStopLossPercent(entryPrice, option.getAsk());
    bool SLACheck = includeStopLossAmount && checkStopLossAmount(entryPrice, option.getAsk());

    shouldClose = timeCheck || mispriceCheck || SLACheck || SLPCheck;

    return shouldClose;
}