//
// Created by TieMajor on 26/11/2021.
//

#ifndef BENCHER_IBENCH_HPP
#define BENCHER_IBENCH_HPP

#include <string>
#include <map>

namespace benchy {
    class IBench {
    public:
        virtual ~IBench() = default;

        virtual std::string getName() = 0;
        virtual std::map<std::string, std::string> doBenchmark() = 0;
    };
}



#endif //BENCHER_IBENCH_HPP
