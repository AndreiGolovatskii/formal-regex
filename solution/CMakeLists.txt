cmake_minimum_required(VERSION 3.4)
project(solution)

include_directories(${nfa_SOURCE_DIR})
include_directories(${rpn_SOURCE_DIR})

add_library(solution solution.h solution.cpp)
target_link_libraries(solution nfa)
target_link_libraries(solution rpn)

