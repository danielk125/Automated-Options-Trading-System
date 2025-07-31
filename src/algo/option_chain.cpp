#include "../../include/option_chain.h"

using std::cout;

OptionChain::OptionChain(string filename){
    string path = filename.substr(filename.find_first_of("_") + 1);

    int symbolStart = 4; // Index of first char not part of file structure is always 9
    int symbolEnd = path.find_first_of("_");
    string symbol = path.substr(symbolStart, symbolEnd - symbolStart);

    std::ifstream file(filename);

    int i = 0;
    string rawLine;
    while(std::getline(file, rawLine)){
        vector<string> rawOptionData;
        std::stringstream line(rawLine);
        string field;

        while(std::getline(line, field, ','))
            rawOptionData.push_back(field);

        double assetPrice =     std::stof(rawOptionData[0]);
        int year =              std::stoi(rawOptionData[1]);
        int month =             std::stoi(rawOptionData[2]);
        int day =               std::stoi(rawOptionData[3]);
        double strikePrice =    std::stof(rawOptionData[4]);
        double bid =            std::stof(rawOptionData[5]);
        double ask =            std::stof(rawOptionData[6]);
        int bidSize =           std::stoi(rawOptionData[7]);
        int askSize =           std::stoi(rawOptionData[8]);
        int volume =            std::stoi(rawOptionData[9]);
        double rho =            std::stof(rawOptionData[10]);
        double vega =           std::stof(rawOptionData[11]);
        double theta =          std::stof(rawOptionData[12]);
        double delta =          std::stof(rawOptionData[13]);
        double gamma =          std::stof(rawOptionData[14]);
        double iv =             std::stof(rawOptionData[15]);

        string dateID  =  std::to_string(month) + '_' +
                          std::to_string(day) + '_' +
                          std::to_string(year);
        
        _chain[dateID].emplace_back(
            OptionType::CALL, 
            assetPrice,
            year,
            month,
            day,
            strikePrice, 
            bid, 
            ask, 
            bidSize, 
            askSize, 
            volume, 
            rho, 
            vega, 
            theta, 
            delta, 
            gamma, 
            iv
        );

        if (i == 0){
            Asset a(symbol, assetPrice);
            _a = a;
        }

        i++;
    }
}

OptionChain::OptionChain(string option_filename, double assetPrice, string symbol){
    Asset a(symbol, assetPrice);
    _a = a;
    std::ifstream o_file(option_filename);
    string rawLine;
    while(std::getline(o_file, rawLine)){
        vector<string> rawOptionData;
        std::stringstream line(rawLine);
        string field;

        while(std::getline(line, field, ','))
            rawOptionData.push_back(field);

        if (rawOptionData[0] == "contractID") // first line
            continue;

        string date = rawOptionData[2];
        string s_year = date.substr(0, date.find_first_of('-'));
        string s_month = date.substr(date.find_first_of('-') + 1, date.find_last_of('-'));
        string s_day = date.substr(date.find_last_of('-'));

        string s_type = rawOptionData[4];

        OptionType type =       s_type == "call" ? OptionType::CALL : OptionType::PUT;
        int year =              std::stoi(s_year);
        int month =             std::stoi(s_month);
        int day =               std::stoi(s_day);
        double strikePrice =    std::stof(rawOptionData[3]);
        double bid =            std::stof(rawOptionData[7]);
        double ask =            std::stof(rawOptionData[9]);
        int bidSize =           std::stoi(rawOptionData[8]);
        int askSize =           std::stoi(rawOptionData[10]);
        int volume =            std::stoi(rawOptionData[11]);
        double rho =            std::stof(rawOptionData[19]);
        double vega =           std::stof(rawOptionData[18]);
        double theta =          std::stof(rawOptionData[17]);
        double delta =          std::stof(rawOptionData[15]);
        double gamma =          std::stof(rawOptionData[16]);
        double iv =             std::stof(rawOptionData[14]);

        string dateID  =  date;
        
        _chain[dateID].emplace_back(
            type, 
            assetPrice,
            year,
            month,
            day,
            strikePrice, 
            bid, 
            ask, 
            bidSize, 
            askSize, 
            volume, 
            rho, 
            vega, 
            theta, 
            delta, 
            gamma, 
            iv
        );
    }
}

unordered_map<string, vector<Option>>* OptionChain::getChain(){
    return &_chain;
}

void OptionChain::printChain(string& dateID){
    if(!_chain.contains(dateID))
        return;

    cout << "Option Chain for " << _a.symbol << ", dateID: " << dateID << ":\n";

    for(Option o : _chain[dateID]){
        o.printOption();
    }

    cout << "END\n";
}

void OptionChain::printChain(){
    cout << "Option Chain for " << _a.symbol << '\n';

    for(auto &o_vector : _chain)
        for(Option o : o_vector.second)
            o.printOption();
    
}

string OptionChain::getSymbol(){
    return _a.symbol;
}

Asset OptionChain::getAsset(){
    return _a;
}