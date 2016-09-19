cmake_minimum_required(VERSION 2.8)

project(Calculator CXX)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

# Allow for includes from top level.
include_directories(.)

# The core library
add_subdirectory(libcalc)

# The calculator application.
add_subdirectory(calc)

# Tests.
add_subdirectory(tests)
## Lexer test.
add_test(test-lex tests/lex ${CMAKE_SOURCE_DIR}/tests/inputs/lex.txt)
## Parser test.
add_test(test-parse tests/parse ${CMAKE_SOURCE_DIR}/tests/inputs/parse.txt)
## Eval test.
add_test(test-eval tests/eval ${CMAKE_SOURCE_DIR}/tests/inputs/eval.txt)