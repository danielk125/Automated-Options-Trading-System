#include "../../include/algo.h"

bool Algo::mispriced(Option& o, int& direction){
    double up_margin = 0.01;
    double down_margin = 0.01;

    if (o._ask < o._fairValue){
        double diff = o._fairValue - o._ask;
        double percent_diff = diff / o._ask;

        if (percent_diff > up_margin){
            return true;
        }

        return false;

    } else if (o._bid > o._fairValue) {
        double diff = o._bid - o._fairValue;
        double percent_diff = diff / o._bid;

        if (percent_diff > down_margin){
            return true;
        }

        return false;
        
    } else {
        return false;
    }
}
