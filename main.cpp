//
// Created by TieMajor on 26/11/2021.
//

#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include <iostream>
#include "IBench.hpp"
#include "FurMarkBench.hpp"

void loadBenchmarks(std::vector<std::shared_ptr<benchy::IBench>> &benchmarks) {
    benchmarks.push_back(std::make_shared<benchy::FurMarkBench>());
}

int main(int ac, char **av) {
    std::vector<std::shared_ptr<benchy::IBench>> benchmarks;

    loadBenchmarks(benchmarks);

    for (const auto &bench : benchmarks) {
        auto res = bench->doBenchmark();
        for(auto &r : res)
        {
            std::cout << "Key : " << r.first << std::endl << "Value : " << r.second << std::endl;
        }
    }

    return 0;
}