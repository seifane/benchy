cmake_minimum_required(VERSION 3.0)
project(bencher)

set(CMAKE_CXX_STANDARD 23)

add_executable(bencher main.cpp helpers.hpp FurMarkBench.cpp)
