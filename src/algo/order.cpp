#include "../../include/order.h"

Order::Order(const Option& option, OrderSide side, OrderType type, double price)
    : _option(option), _side(side), _type(type), _price(price)
{
    ORDER_COUNT++;
}

int Order::get_price(){
    return _price;
}

OrderType Order::get_order_type(){
    return _type;
}

Order::~Order(){
    ORDER_COUNT--;
}
