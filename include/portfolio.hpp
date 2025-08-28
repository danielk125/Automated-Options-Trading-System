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
#include "closing_filter.hpp"

using json = nlohmann::json;

struct Position {
    PositionSide posSide;
    SecurityType secType;
    double entryPrice;
    std::unique_ptr<Abreviator> secInfo;
    double curValue;
    int size;

    Position() = default;
    Position(const Position&)            = delete;
    Position& operator=(const Position&) = delete;
    Position(Position&&)                 = default;
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
    void loadPortfolio();
    void executeOrders();

    void loadPortfolioBacktest();
    void savePortfolioBacktest();

    double calculateCurrentValue() const;

private:
    OptionMap& _OMap;
    vector<Position> _currentPositions;
    ClosingFilter _filter;
    double cash;
    double value;
    double unrealizedGain;
    double realizedGain;

    vector<Order> closePositions();
    vector<Order> openPositions();

    void loadClosingFilter();
    double closePositionsBacktest(double& gain);
    void updatePositionsBacktest();
    double openPositionsBacktest();
    double calculateUnrealizedGain();
};
