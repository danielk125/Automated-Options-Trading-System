#pragma once

#include <string>
#include "util.h"
#include "option.h"

struct Position {

    Option o;
    OrderSide side;
    double price;
    int quantity;

};
