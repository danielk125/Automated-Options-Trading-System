#include "../../include/option_map.hpp"

using std::cout;

void OptionMap::loadFilter(string filter_filename){
    string filename = "filters/" + filter_filename + ".csv";

    std::ifstream file(filename);

    string rawline;
    std::getline(file, rawline);
    string field;
    std::stringstream line(rawline);
    vector<string> fields;

    while(std::getline(line, field, ','))
        fields.push_back(field);
    
    int numWeeksToExpirationStart       = std::stoi(fields[0]);
    int numWeeksToExpirationEnd         = std::stoi(fields[1]);
    double mispriceThresholdPercent     = std::stod(fields[2]);
    bool includeCalls                   = std::stoi(fields[3]);
    bool includePuts                    = std::stoi(fields[4]);
    bool includeBuys                    = std::stoi(fields[5]);
    bool includeSells                   = std::stoi(fields[6]);

    _filter = OptionFilter(numWeeksToExpirationStart, numWeeksToExpirationEnd, mispriceThresholdPercent, includeCalls, includePuts, includeBuys, includeSells);
}

OptionMap::OptionMap(string symbol, bool customFilter) : _symbol(symbol) {
    loadFilter(customFilter ? symbol : "filter");

    string filename = "live_CSV/" + symbol + "_option_chain.csv";

    std::ifstream file(filename);

    string rawLine;
    while(std::getline(file, rawLine)){
        vector<string> rawOptionData;
        std::stringstream line(rawLine);
        string field;

        while(std::getline(line, field, ','))
            rawOptionData.push_back(field);

        double assetPrice =     std::stod(rawOptionData[0]);
        int year =              std::stoi(rawOptionData[1]);
        int month =             std::stoi(rawOptionData[2]);
        int day =               std::stoi(rawOptionData[3]);

        Option call;
        Option put;
        int num_o_params = 13;

        for (int i = 0; i < 2; ++i){
            int start = i * num_o_params;

            double strikePrice =    std::stod(rawOptionData[start + 4]);
            double bid =            std::stod(rawOptionData[start + 5]);
            double ask =            std::stof(rawOptionData[start + 6]);
            int bidSize =           std::stoi(rawOptionData[start + 7]);
            int askSize =           std::stoi(rawOptionData[start + 8]);
            int volume =            std::stoi(rawOptionData[start + 9]);
            int openInterest =      std::stoi(rawOptionData[start + 10]);
            double rho =            std::stod(rawOptionData[start + 11]);
            double vega =           std::stod(rawOptionData[start + 12]);
            double theta =          std::stod(rawOptionData[start + 13]);
            double delta =          std::stod(rawOptionData[start + 14]);
            double gamma =          std::stod(rawOptionData[start + 15]);
            double iv =             std::stod(rawOptionData[start + 16]);

            if (i == 0){
                call = Option(CALL, assetPrice, year, month, day, strikePrice, bid, ask, bidSize, askSize, volume, openInterest, rho, vega, theta, delta, gamma, iv);
            } else {
                put = Option(PUT, assetPrice, year, month, day, strikePrice, bid, ask, bidSize, askSize, volume, openInterest, rho, vega, theta, delta, gamma, iv);
            }
        }

        bool validOption = _filter.markOption(call);
        if (validOption)
            _filtered.push(call);
        validOption = _filter.markOption(put);
        if (validOption)
            _filtered.push(put);
        
        string dateID = call.getExpDate();
        double k = call.getStrike();

        auto& chain = _map[dateID];
        chain.insert_or_assign(k, OptionPair(call, put));
    }
}

const OptionChain OptionMap::operator[](std::string_view dateID){
    return OptionChain(_map.at(static_cast<string>(dateID)));
}

void OptionMap::printChain() const {
    for (const auto& chain : _map){
        for (const auto& pair : chain.second){
            pair.second.getCall().printOption();
            pair.second.getPut().printOption();
        }
    }
}

std::optional<Option> OptionMap::getFilteredContract() {
    if (_filtered.empty())
        return std::nullopt;

    Option o = _filtered.top();
    _filtered.pop();
    return o;
}

const unordered_map<double, OptionPair>& OptionMap::getSingleChain(std::string_view dateID) const{
    return _map.at(static_cast<string>(dateID));
}

double OptionMap::accessMispriceThreshold(){
    return _filter.getMisprice();
}