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

    int i = 0;
    for(auto n : *(chain)){
        for(auto o : n.second){
            if(i%20 == 0)
                o.printOption();
        }
    }
    
    Algo a;
    auto rel = a.extractContracts(o_chain, 2, 6);

    for (auto& o : rel[BUY]){
        std::cout << ("BUYS:");
        o.printOption();
    }
        
    for (auto& o : rel[SELL]){
        std::cout << ("SELLS:");
        o.printOption();
    }

    return 0;
}
