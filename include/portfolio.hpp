#include <vector>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <span>

#include "json.hpp"
#include "util.hpp"
#include "option_map.hpp"

using json = nlohmann::json;

struct Position {
    OrderSide side;
    double entryPrice;
    OptionAbrv contractInfo;
    double curValue;
    int size;
};

class Portfolio {
public:
    // parse current positions and apply closing logic
    vector<Position&> closePositions();

    // parse relevent contracts and enter new positions
    vector<Position&> openPositions();

    // send positions to execution handler
    void executeOrders();

    // for backtesting
    void loadPortfolio();

    void savePortfolio();

private:
    OptionMap _OMap;

    vector<Position> _currentPositions;

    double cash;
    double value;
    double unrealizedGain;
    double realizedGain;

    void parseBalance(double& cash, double& balance);
};
