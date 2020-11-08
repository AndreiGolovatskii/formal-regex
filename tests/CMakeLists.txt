cmake_minimum_required(VERSION 3.4)
project(test)

include_directories(${nfa_SOURCE_DIR})
include_directories(${rpn_SOURCE_DIR})
include_directories(${solution_SOURCE_DIR})

set(TEST_SOURCES test_nfa.cpp test_rnp.cpp test_solution.cpp)

enable_testing()
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests nfa)
target_link_libraries(Tests rpn)
target_link_libraries(Tests solution)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} pthread)
