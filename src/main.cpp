#include<iostream>
#include "../include/option.h"
#include "../include/option_chain.h"
#include <chrono>

int main(){
    string filename = "../src/rawCSV/NVDA_option_chain.csv";

    auto start = std::chrono::high_resolution_clock::now();
    OptionChain NVDA_chain(filename);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Option chain construction took " << duration.count() << "seconds\n";

    return 0;
}
