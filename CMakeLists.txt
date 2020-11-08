cmake_minimum_required(VERSION 3.4)
project(practicum)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)
include(CodeCoverage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -Werror -pedantic -std=c++17")
append_coverage_compiler_flags()


add_subdirectory(nfa)
add_subdirectory(rpn)
add_subdirectory(solution)
add_subdirectory(tests)

setup_target_for_coverage_gcovr_html(
        NAME gtests_cov
        EXECUTABLE Tests
        DEPENDENCIES Tests
)

add_custom_command(
        TARGET gtests_cov
        COMMAND find . -name "*.gcda" -print0 | xargs -0 rm
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "cleaning .gcda..."
)
