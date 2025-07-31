#include "../../include/algo.h"

bool Algo::mispriced(const Option& o, OrderSide& direction){
    double up_margin = 0.001;
    double down_margin = 0.001;

    if (o._ask < o._fairValue){
        double diff = o._fairValue - o._ask;
        double percent_diff = diff / o._ask;

        if (percent_diff > up_margin){
            direction = BUY;
            return true;
        }
        return false;

    } else if (o._bid > o._fairValue) {
        double diff = o._bid - o._fairValue;
        double percent_diff = diff / o._bid;

        if (percent_diff > down_margin){
            direction = SELL;
            return true;
        }
        return false;
        
    } else {
        return false;
    }
}

std::unordered_map<OrderSide, std::vector<Option>> Algo::extractContracts(OptionChain& oc, int startWeek, int endWeek){
    double price = oc.getAsset().price;
    double target_threshold = 2;
    double target = price * target_threshold + price;
    auto chain = oc.getChain(); // unordered map

    std::unordered_map<OrderSide, std::vector<Option>> relevants;
    relevants[BUY];
    relevants[SELL];
    int i = 0;
    for (auto& node : *chain){
        if (i >= startWeek && i <= endWeek){
            auto options = node.second;

            for (Option& contract : options){
                double k = contract._strikePrice;
                
                if (k > price)
                    break;

                if (k < target)
                    continue;

                OrderSide direction;
                if (mispriced(contract, direction)){
                    relevants[direction].push_back(contract);
                }
            }
        }
        
        ++i;
    }

    return relevants;
}