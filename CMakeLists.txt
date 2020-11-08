cmake_minimum_required(VERSION 3.17)
project(practicum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_library(nfa nfa.h nfa.cpp)

set(TEST_SOURCES main.cpp solution.h)

enable_testing()
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests nfa)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} pthread)



