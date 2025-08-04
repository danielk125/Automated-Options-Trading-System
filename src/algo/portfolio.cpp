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


void Portfolio::savePortfolio(){
    json j;
    j["cash"] = _cash;
    j["value"] = _value;

    for (const auto& [symbol, pos_list] : _positions) {
        for (const auto& pos : pos_list) {
            json pos_json;

            const Option& option = pos.o;

            pos_json["option"] = {
                {"optionType", option._optionType},
                {"assetPrice", option._assetPrice},
                {"year", option._year},
                {"month", option._month},
                {"day", option._day},
                {"strikePrice", option._strikePrice},
                {"bid", option._bid},
                {"ask", option._ask},
                {"bidSize", option._bidSize},
                {"askSize", option._askSize},
                {"volume", option._volume},
                {"rho", option._greeks.rho},
                {"vega", option._greeks.vega},
                {"theta", option._greeks.theta},
                {"delta", option._greeks.delta},
                {"gamma", option._greeks.gamma},
                {"iv", option._impliedVol}
            };

            pos_json["quantity"] = pos.quantity;
            pos_json["price"] = pos.price;

            j["positions"][symbol].push_back(pos_json);
        }
    }

    std::ofstream file("portfolio.json");
    file << std::setw(2) << j << std::endl;
}