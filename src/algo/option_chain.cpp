#include "../../include/option_chain.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;

OptionChain::OptionChain(string filename){
    int symbolStart = 14; // Index of first char not part of file structure is always 14
    int symbolEnd = filename.find_first_of("_");
    _symbol = filename.substr(symbolStart, symbolEnd - symbolStart);

    cout << _symbol << '\n';

    std::ifstream file(filename);

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
        bool curValue =         rawOptionData[16] == "True";
        string quoteDetail =    rawOptionData[17];

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
            iv, 
            curValue, 
            quoteDetail
        );
    }
}


unordered_map<string, vector<Option>>* OptionChain::getChain(){
    return &_chain;
}

void OptionChain::printChain(string& dateID){
    if(!_chain.contains(dateID))
        return;

    cout << "Option Chain for " << _symbol << ", dateID: " << dateID << ":\n";

    for(Option o : _chain[dateID]){
        o.printOption();
    }

    cout << "END\n";
}

void OptionChain::printChain(){
    cout << "Option Chain for " << _symbol << '\n';

    for(auto &o_vector : _chain)
        for(Option o : o_vector.second)
            o.printOption();
    
}
