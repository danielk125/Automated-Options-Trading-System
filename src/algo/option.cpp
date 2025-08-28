#include "../../include/option.hpp"

using std::cout;

void Option::calculateTimeToExpiration(string startDate){
    time_t now; 

    if (startDate == "") now = time(NULL);
    else {
        int sMonth = std::stoi(startDate.substr(0,2));
        int sDay = std::stoi(startDate.substr(3,2));
        int sYear = std::stoi(startDate.substr(6,4));
        struct tm startTime {0};
        startTime.tm_year = sYear - 1900;
        startTime.tm_mon = sMonth - 1;
        startTime.tm_mday = sDay;
        startTime.tm_hour = 16;
        startTime.tm_min = 30;
        startTime.tm_sec = 0;
        startTime.tm_isdst = -1;

        now = mktime(&startTime);
    }

    struct tm expTime {0};
    expTime.tm_year = _year - 1900;
    expTime.tm_mon = _month - 1;
    expTime.tm_mday = _day;
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

void Option::calculate_binomial_params (
    int& N,
    double& r,
    double& dt,
    double& u,
    double& d,
    double& p,
    double& df,
    const double& T,
    const double& sigma
) const {
    N = 100;
    r = 0.05;

    dt = T / N;
    u = std::exp(sigma * std::sqrt(dt));
    d = 1 / u;
    p = (std::exp(r * dt) - d) / (u - d);
    df = std::exp(-r * dt);
}

double Option::calculatePayoff(double val, double K, OptionType type) const{
    if (type == CALL){
        return std::max(0.0, val - K);
    } else {
        return std::max(0.0, K - val);
    }
}

double Option::price_binomial(){
    OptionType type = _optionType;
    double S = _assetPrice;
    double K = _strikePrice;
    double T = _eTime;
    double sigma = _impliedVol;
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

Option::Option(){
    _optionType     = CALL;
    _assetPrice     = -1;
    _strikePrice    = -1;
    _bid            = -1;
    _ask            = -1;
    _bidSize        = -1;
    _askSize        = -1;
    _volume         = -1;
    _openInterest   = -1;
    _rho            = -1;
    _vega           = -1;
    _theta          = -1;
    _delta          = -1;
    _gamma          = -1;
    _impliedVol     = -1;
    _expirationDate = "";
    _eTime          = -1;
    _fairValue      = -1;
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
    int openInterest,
    double rho, 
    double vega, 
    double theta, 
    double delta, 
    double gamma, 
    double iv,
    string startDate
    ) : 
    _optionType(optionType),
    _assetPrice(assetPrice),
    _strikePrice(strikePrice), 
    _bid(bid), 
    _ask(ask), 
    _bidSize(bidSize), 
    _askSize(askSize), 
    _volume(volume), 
    _openInterest(openInterest),
    _impliedVol(iv),
    _delta(delta),
    _gamma(gamma),
    _theta(theta),
    _rho(rho),
    _vega(vega),
    _day(day),
    _month(month),
    _year(year)
{
    _expirationDate =   std::to_string(month) + "_" +
                        std::to_string(day)  + "_" +
                        std::to_string(year);
    calculateTimeToExpiration(startDate);
    _fairValue = price_binomial();
    calculateMisprice();
    
}

void Option::printOption() const  {
    cout << (_optionType ? "PUT OPTION, " : "CALL OPTION, ")
        << "Expiration Date: " << _expirationDate
        << ", Underlying Price: " << _assetPrice
        << ", Strike Price: " << _strikePrice 
        << ", Ask Value: " << _ask
        << ", Bid Value: " << _bid
        << ", Fair value: " << _fairValue
        << ", Misprice Buy: " << _mispricePercentBuy
        << ", Misprice Sell: " << _mispricePercentSell << '\n';
}

void Option::calculateMisprice() {
    _mispricePercentBuy = (_fairValue - _ask) / _ask;
    _mispricePercentSell = (_bid - _fairValue) / _bid; 
}

string Option::getExpDate() const { return _expirationDate; }
double Option::getStrike() const { return _strikePrice; }
OptionType Option::getOptionType() const { return _optionType; }
double Option::getAssetPrice() const { return _assetPrice; }
double Option::getBid() const { return _bid; }
double Option::getAsk() const { return _ask; }
int Option::getVolume() const { return _volume; }
int Option::getOI() const { return _openInterest; }
int Option::getBidSize() const { return _bidSize; }
int Option::getAskSize() const { return _askSize; }
double Option::getIV() const { return _impliedVol; }
double Option::timeToExpiration() const { return _eTime; }
double Option::fairValue() const { return _fairValue; }
double Option::getDelta() const { return _delta; }
double Option::getGamma() const { return _gamma; }
double Option::getTheta() const { return _theta; }
double Option::getRho() const { return _rho; }
double Option::getVega() const { return _vega; }
double Option::getMispriceBuy() const { return _mispricePercentBuy; }
double Option::getMispriceSell() const { return _mispricePercentSell; }
bool Option::checkBuy() const { return _buy; }
bool Option::checkSell() const { return _sell; }

void Option::markBuy(bool b){
    _buy = b;
}
void Option::markSell(bool s){
    _sell = s;
}