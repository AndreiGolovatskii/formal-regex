cmake_minimum_required(VERSION 3.1)
project(practicum)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)
include(CodeCoverage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -Werror -pedantic -std=c++17")
append_coverage_compiler_flags()


add_subdirectory(nfa)
add_subdirectory(tests)

setup_target_for_coverage_gcovr_html(
  NAME gtests_cov
  EXECUTABLE Tests
  DEPENDENCIES Tests
  BASE_DIRECTORY ${nfa_SOURCE_DIR}
)
