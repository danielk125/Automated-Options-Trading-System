#include "../../include/algo.h"

double Algo::timeToExpiration(int e_year, int e_month, int e_day){
    time_t now = time(NULL);

    struct tm expTime {0};
    expTime.tm_year = e_year - 1900;
    expTime.tm_mon = e_month - 1;
    expTime.tm_mday = e_day;
    expTime.tm_hour = 16;
    expTime.tm_min = 30;
    expTime.tm_sec = 0;
    expTime.tm_isdst = -1;

    time_t expDate = mktime(&expTime);

    double diff = difftime(expDate, now);
    const double SEC_PER_YEAR = 31536000.0;

    double timeToExpireYears = static_cast<double>(diff) / SEC_PER_YEAR;

    return timeToExpireYears;
}

void Algo::calculate_binomial_params(
    int& N,
    double& r,
    double& dt,
    double& u,
    double& d,
    double& p,
    double& df,
    const double& T,
    const double& sigma
) {
    N = 100;
    r = 0.05;

    dt = T / N;
    u = std::exp(sigma * std::sqrt(dt));
    d = 1 / u;
    p = (std::exp(r * dt) - d) / (u - d);
    df = std::exp(-r * dt);
}

double Algo::calculatePayoff(double val, double K, OptionType type){
    if (type == CALL){
        return std::max(0.0, val - K);
    } else {
        return std::max(0.0, K - val);
    }
}

double Algo::price_binomial(Option& o) {
    OptionType type = o._optionType;
    double S = o._assetPrice;
    double K = o._strikePrice;
    double T = timeToExpiration(o._year, o._month, o._day);
    double sigma = o._greeks.iv;
    int N;
    double r, dt, u, d, p, df;

    calculate_binomial_params(N, r, dt, u, d, p, df, T, sigma);

    std::vector<double> optionValues(N + 1);

    for (int i = 0; i <= N; i++){
        double val = S * std::pow(u, N - i) * std::pow(d,i);
        optionValues[i] = calculatePayoff(val, K, type);
    }

    for (int step = N - 1; step >= 0; step--){
        for (int i = 0; i <= step; i++){
            optionValues[i] = (p * optionValues[i] + (1-p) * optionValues[i+1]) * df;

            double val = S * std::pow(u, step - i) * std::pow(d, i);
            optionValues[i] = std::max(optionValues[i], calculatePayoff(val, K, type));
        }
    }

    return optionValues[0];
}