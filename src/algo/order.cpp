#include "../../include/order.h"

Order::Order(OrderSide side, int amount, OrderType type, int price, int user_id)
    : _side(side), _amount(amount), _type(type), _price(price), _user_id(user_id)
{
    _status = OPEN;
    _order_id = ++ORDER_COUNT;
    OPEN_COUNT++;
}

int Order::get_price(){
    return _price;
}

OrderType Order::get_order_type(){
    return _type;
}

void Order::set_status(OrderStatus status){
    _status = status;
}

Order::~Order(){
    ORDER_COUNT--;

    if(_status == OPEN)
        OPEN_COUNT--;
    else if(_status == FILLED)
        FILLED_COUNT--;
}
