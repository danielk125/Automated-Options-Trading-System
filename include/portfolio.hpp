#include <vector>
#include <unordered_map>
#include <fstream>
#include <string_view>
#include <span>
#include <fstream>
#include <utility>
#include <iostream>
#include <filesystem>

#include "json.hpp"
#include "util.hpp"
#include "option_map.hpp"

using json = nlohmann::json;

struct Position {
    PositionSide posSide;
    SecurityType secType;
    double entryPrice;
    std::unique_ptr<Abreviator> secInfo;
    double curValue;
    int size;

    Position() = default;
    Position(const Position&)            = delete;    // no copying
    Position& operator=(const Position&) = delete;
    Position(Position&&)                 = default;   // movable
    Position& operator=(Position&&)      = default;
};

struct Order {
    OrderSide ordSide;
    SecurityType secType;
    double price;
    std::unique_ptr<Abreviator> secInfo;
    int size;

    Order() = default;
    Order(const Order&)            = delete;
    Order& operator=(const Order&) = delete;
    Order(Order&&)                 = default;
    Order& operator=(Order&&)      = default;
};

class Portfolio {
public:
    Portfolio(OptionMap& omap);
    // parse current positions and apply closing logic
    vector<Order> closePositions();

    // parse relevent contracts and enter new orders
    vector<Order> openPositions();

    // send positions to execution handler
    void executeOrders();

    // for backtesting
    void loadPortfolio();

private:
    OptionMap& _OMap;

    vector<Position> _currentPositions;

    double cash;
    double value;
    double unrealizedGain;
    double realizedGain;
};
