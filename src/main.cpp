#include<iostream>
#include "../include/option.h"
#include "../include/option_chain.h"
#include "../include/algo.h"
#include <chrono>
#include <print>

int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;
    string filename = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    OptionChain o_chain(filename);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << o_chain.getSymbol() << " option chain construction took " << duration.count() << " seconds\n";

    unordered_map<string, vector<Option>>* chain = o_chain.getChain();
    std::string_view sym(o_chain.getSymbol());
    
    Algo a;
    auto rel = a.extractContracts(o_chain, 4, 10);
    auto posits = a.generatePositions(rel);

    Portfolio p;
    p.loadPortfolio();
    p.addPositions(sym, posits);
    p._cash = 1000;
    p.calculateValue();
    p._gain = 0;

    p.savePortfolio();

    return 0;
}
