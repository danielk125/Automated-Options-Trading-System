#include "option.h"
#include "order.h"

#include <ctime>
#include <vector>
#include <cmath>

class Algo{

    double timeToExpiration(int e_year, int e_month, int e_day);

    void calculate_binomial_params(
        int& N,
        double& r,
        double& dt,
        double& u,
        double& d,
        double& p,
        double& df,
        const double& T,
        const double& sigma
    );

    double calculatePayoff(double val, double K, OptionType type);

public:

    double price_binomial(Option& o);

};
