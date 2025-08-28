#pragma once

#include "util.hpp"
#include "option.hpp"

class ClosingFilter {
    bool includeAutoSell;
    int numWeeksFromExpiration;
    bool includeClosingThreshold;
    double mispriceClosingThreshold;
    bool includeStopLossPercent;
    double stopLossPercent;
    bool includeStopLossAmount;
    double stopLossAmount;

    bool checkTime(double timeToExpiration);

    bool checkMisprice(double misprice);

    bool checkStopLossPercent(double initialCost, double currentvalue);

    bool checkStopLossAmount(double initialCost, double currentvalue);

public:
    ClosingFilter();

    ClosingFilter(bool ias, int nwe, bool ict, double mpct, bool islp, double slp, bool isla, double sla);

    bool markPosition(PositionSide pside, double entryPrice, Option& option);
};