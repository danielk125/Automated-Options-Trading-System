#include<iostream>
#include "../include/option.h"
#include "../include/option_chain.h"
#include "../include/algo.h"
#include <chrono>

int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;
    string filename = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    OptionChain o_chain(filename);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << *o_chain.getSymbol() << " option chain construction took " << duration.count() << " seconds\n";

    unordered_map<string, vector<Option>>* chain = o_chain.getChain();
    Algo algo;

    int i = 0;
    start = std::chrono::high_resolution_clock::now();
    for(auto n : *(chain)){
        for(auto o : n.second){
            Order order = algo.price_binomial(o);
            if(i%20 == 0)
                std::cout << "Market value: " << o.getprice() << "; Fair value: " << order.get_price() << '\n';
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << *o_chain.getSymbol() << " option chain pricing took " << duration.count() << " seconds\n";

    return 0;
}
