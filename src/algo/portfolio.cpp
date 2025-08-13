#include "../../include/portfolio.hpp"

vector<Position&> Portfolio::closePositions(){
    
}

void Portfolio::parseBalance(double& cash, double& balance){

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
                auto side = pos["side"];

                Position p (o, side, price, quantity);

                _positions[sym.key()].push_back(p);
            }
        }
    }
}
