#include <vector>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <span>

#include "json.hpp"
#include "util.hpp"

using json = nlohmann::json;

struct Position {
    OrderSide side;
    double entryPrice;
    OptionAbrv contractInfo;
    double curValue;
};

class Portfolio {
public:
    double cash;
    double value;
    double unrealizedGain;
    double realizedGain;

    void calculateValue();

    void calculateGain(Portfolio& other);

    void closePositions();

    void addPositions(std::string_view symbol, std::span<Position> positions);

    void loadPortfolio();

    void savePortfolio();

    friend class Algo;
};
