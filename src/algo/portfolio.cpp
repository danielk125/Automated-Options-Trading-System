#include "../../include/portfolio.h"

void Portfolio::calculateValue(){
    double value = 0;

    for (auto& pos_list : _positions){
        for (auto pos : pos_list.second){
            value += pos.price * pos.quantity;
        }
    }

    _value = value;
}

void Portfolio::calculateGain(Portfolio& other){
    _gain = _value - other._value + other._gain;
}

void Portfolio::loadPortfolio(){
    if (std::filesystem::exists("portfolio.json")) {
        auto j = json::parse(std::ifstream("portfolio.json"));
        _cash = j["cash"];
        _value = j["value"];

        for (auto& sym : j["positions"].items()) {
            for (auto& pos : sym.value()){
                auto option = pos["option"];
                
                Option o(
                    option["optionType"],
                    option["assetPrice"],
                    option["year"],
                    option["month"],
                    option["day"],
                    option["strikePrice"],
                    option["bid"],
                    option["ask"],
                    option["bidSize"],
                    option["askSize"],
                    option["volume"],
                    option["rho"],
                    option["vega"],
                    option["theta"],
                    option["delta"],
                    option["gamma"],
                    option["iv"]
                );

                auto quantity = pos["quantity"];
                auto price = pos["price"];

                Position p (o, price, quantity);

                _positions[sym.key()].push_back(p);
            }
        }
    }
}
