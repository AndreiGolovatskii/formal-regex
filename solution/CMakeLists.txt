cmake_minimum_required(VERSION 3.1)
project(solution)

include_directories(${nfa_SOURCE_DIR})

add_library(solution solution.h solution.cpp)
