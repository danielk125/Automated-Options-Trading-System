#include<iostream>
#include "../include/portfolio.hpp"
#include <chrono>
#include <print>

int main(int argc, char* argv[]){
    if(argc != 2)
        return 1;
    string symbol = argv[1];

    auto start = std::chrono::high_resolution_clock::now();
    OptionMap o(symbol, false);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << symbol << " option chain construction took " << duration.count() << " seconds\n";

    Portfolio p(o);
    p.loadPortfolio();
    p.executeOrders();

    return 0;
}
