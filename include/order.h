#pragma once

#include <string>
#include "util.h"
#include "option.h"
class Order {
    inline static int ORDER_COUNT = 0;     // static member for tracking the total number of orders ever

    const Option& _option;
    OrderSide _side;
    OrderType _type;
    double _price;

public:
    Order(const Option& option, OrderSide side, OrderType type, double price);

    double get_price();

    OrderType get_order_type();

    ~Order();
};