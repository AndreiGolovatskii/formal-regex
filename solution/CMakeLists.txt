cmake_minimum_required(VERSION 3.4)
project(solution)

include_directories(${nfa_SOURCE_DIR})

add_library(solution solution.h solution.cpp)
