cmake_minimum_required(VERSION 3.4)
project(test)

include_directories(${nfa_SOURCE_DIR})
include_directories(${rpn_SOURCE_DIR})

set(TEST_SOURCES test_nfa.cpp test_rnp.cpp)

enable_testing()
add_executable(Tests ${TEST_SOURCES})
target_link_libraries(Tests nfa)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} pthread)
