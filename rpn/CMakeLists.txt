cmake_minimum_required(VERSION 3.4)
project(rpn)

include_directories(${nfa_SOURCE_DIR})
add_library(rpn rpn.h rpn.cpp)
