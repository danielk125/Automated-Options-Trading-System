#pragma once

#include "util.hpp"
#include "option.hpp"

class OptionFilter {
    int numWeeksToExpirationStart;
    int numWeeksToExpirationEnd;
    double mispriceThresholdPercent;
    bool includeCalls;
    bool includePuts;
    bool includeBuys;
    bool includeSells;

    bool checkTime(double timeToExpiration);

    bool checkMisprice(double misprice);

    bool checkCall(OptionType ot);

    bool checkPut(OptionType ot);

public:
    OptionFilter();

    OptionFilter(int nws, int nwe, double mpth, bool ic, bool ip, bool ib, bool is);

    bool markOption(Option& option);

    double getMisprice();
};