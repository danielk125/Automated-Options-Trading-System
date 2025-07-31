#include "option.h"
#include "option_chain.h"
#include "position.h"
#include "portfolio.h"

#include <ctime>
#include <vector>
#include <cmath>

class Algo{

    bool mispriced(const Option& o, OrderSide& direction);



public:

    /*
    function to pull relevant contracts
    =+> 1-10% out-of-the-money contracts between 2-6 weeks out
    */ 
   std::unordered_map<OrderSide, std::vector<Option>> extractContracts(OptionChain& oc, int startWeek, int endWeek);

};
