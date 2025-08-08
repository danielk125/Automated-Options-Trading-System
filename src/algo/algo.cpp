#include "../../include/algo.h"

bool Algo::mispriced(const Option& o, OrderSide& direction){ // fix for use with PUTs
    double up_margin = 0.01;
    double down_margin = 0.01;

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
    double target_threshold = 0.1;
    double target = price * target_threshold + price;
    auto chain = oc.getChain(); // unordered map

    std::unordered_map<OrderSide, std::vector<Option>> relevants;
    relevants[BUY];
    relevants[SELL];

    for (int i = startWeek; i <= endWeek; ++i){
        auto options = oc.getSingleChain(i);
        for (Option& contract : options){
            double k = contract._strikePrice;

            if (k < price)
                continue;
            
            if (k > target)
                break;

            OrderSide direction;
            if (mispriced(contract, direction)){
                relevants[direction].push_back(contract);
            }
        }
    }

    return relevants;
}

std::vector<Position> Algo::generatePositions(const std::unordered_map<OrderSide, std::vector<Option>>& contracts){
    std::vector<Position> positions;
    int quantity = 1; // placeholder
    
    for (const auto& o : contracts.at(BUY)){
        positions.emplace_back(
            o,
            BUY,
            o._ask,
            quantity
        );
    }
        
    for (const auto& o : contracts.at(SELL)){
        positions.emplace_back(
            o,
            SELL,
            o._bid,
            quantity
        );
    }
    return positions;
}
