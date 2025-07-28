#include "../../include/option.h"

using std::cout;

void Option::timeToExpiration(int e_year, int e_month, int e_day){
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

    _eTime = timeToExpireYears;
}

void Option::calculate_binomial_params(
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

double Option::calculatePayoff(double val, double K, OptionType type){
    if (type == CALL){
        return std::max(0.0, val - K);
    } else {
        return std::max(0.0, K - val);
    }
}

Option::Option(
    OptionType optionType,
    double assetPrice,
    int year,
    int month,
    int day,
    double strikePrice, 
    double bid, 
    double ask, 
    int bidSize, 
    int askSize, 
    int volume, 
    double rho, 
    double vega, 
    double theta, 
    double delta, 
    double gamma, 
    double iv, 
    bool curValue,
    string quoteDetail
    ) : 
    _optionType(optionType),
    _assetPrice(assetPrice),
    _year(year),
    _month(month),
    _day(day),
    _strikePrice(strikePrice), 
    _bid(bid), 
    _ask(ask), 
    _bidSize(bidSize), 
    _askSize(askSize), 
    _volume(volume), 
    _quoteDetail(quoteDetail),
    _greeks(Greeks(rho, vega, theta, delta, gamma, iv, curValue))
{
    timeToExpiration(year, month, day);
}

void Option::printOption() {
    cout << (_optionType ? "PUT OPTION, " : "CALL OPTION, ")
        << "Expiration Date: " << _month << ' ' << _day << ' ' << _year
        << ", Underlying Price: " << _assetPrice
        << ", Strike Price: " << _strikePrice 
        << ", Market Value: " << _ask;
}

double Option::getprice() {
    return _ask;
}

double Option::price_binomial(){
    OptionType type = _optionType;
    double S = _assetPrice;
    double K = _strikePrice;
    double T = _eTime;
    double sigma = _greeks.iv;
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
