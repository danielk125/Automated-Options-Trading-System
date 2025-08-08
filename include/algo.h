#include "option.h"
#include "option_chain.h"
#include "position.h"
#include "portfolio.h"

#include <ctime>
#include <vector>
#include <cmath>

class Algo{
public:
    bool mispriced(const Option& o, OrderSide& direction);

    /*
    function to add positions to existing portfolio and save
    */
   std::vector<Position> generatePositions(const std::unordered_map<OrderSide, std::vector<Option>>& contracts);

    /*
    function to pull relevant contracts
    =+> 1-10% out-of-the-money contracts between 2-6 weeks out
    */ 
   std::unordered_map<OrderSide, std::vector<Option>> extractContracts(OptionChain& oc, int startWeek, int endWeek);

};
