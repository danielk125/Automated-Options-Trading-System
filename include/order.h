#include <string>

// for reabability
enum OrderSide {
    BUY,
    SELL
};

// possible order status
enum OrderStatus {
    OPEN,
    FILLED,
    PART_FILLED,
    CANCELLED
};

// possible order types
enum OrderType {
    MARKET,
    LIMIT
};

class Order {
    inline static int ORDER_COUNT = 0;     // static member for tracking the total number of orders ever
    inline static int OPEN_COUNT = 0;      // static member for tracking the number of currently open orders
    inline static int FILLED_COUNT = 0;    // static member for tracking the number of filled orders

    int _order_id;              // unique id for each order
    int _user_id;               // who made the order - L3
    int _amount;                // size of the order
    int _price;                 // dollar amount associated with order
    OrderSide _side;            // BUY or SELL
    OrderStatus _status;        // current state of the order
    OrderType _type;            // MARKET or LIMIT (for now)

public:
    // Order constructor
     Order(OrderSide side, int amount, OrderType type, int price, int user_id);

    // return the price per instrument
    int get_price();

    //
    OrderType get_order_type();

    // sets this order's status to CANCELLED
    void set_status(OrderStatus status);

    // Order destructor
    ~Order();

};