//
// Created by TieMajor on 26/11/2021.
//

#ifndef BENCHER_FURMARKBENCH_HPP
#define BENCHER_FURMARKBENCH_HPP

#include "IBench.hpp"

namespace benchy {
    class FurMarkBench: public IBench {
    public:
        FurMarkBench();
        ~FurMarkBench();

        std::string getName();
        std::map<std::string, std::string> doBenchmark();
    };
}

#endif //BENCHER_FURMARKBENCH_HPP
