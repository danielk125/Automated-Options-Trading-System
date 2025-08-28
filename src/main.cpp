#include<iostream>
#include "../include/backtester.hpp"
#include <chrono>
#include <print>

/*
TODO

- make all file paths relative to a project root (cmakelists.txt stuff)

*/

int main(int argc, char* argv[]){
    if(argc != 3) {
        return 1;
    }

    string symbol = argv[1];
    string liveOrBacktest = argv[2];

    if (liveOrBacktest == "live"){
        OptionMap o(symbol, false);
        Portfolio p(o);
        p.loadPortfolio();
        p.executeOrders();
    } else if (liveOrBacktest == "backtest"){
        Backtester B(symbol, false);

        auto start = std::chrono::high_resolution_clock::now();
        B.run();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        std::cout << "backtest took " << duration.count() << " seconds\n";
    }
}
