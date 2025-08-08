#include <vector>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <span>

#include "position.h"
#include "json.h"

using json = nlohmann::json;

struct Portfolio {
    std::unordered_map<std::string,std::vector<Position>> _positions;

    double _cash;
    double _value;
    double _gain;

    void calculateValue();

    void calculateGain(Portfolio& other);

    void closePositions();

    void addPositions(std::string_view symbol, std::span<Position> positions);

    void loadPortfolio();

    void savePortfolio();

    friend class Algo;
};
