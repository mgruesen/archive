cmake_minimum_required(VERSION 2.8)

project(Calculator CXX)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

# Allow for includes from top level.
include_directories(.)

# The core library
add_subdirectory(libcalc)

# The regular calculator application.
add_subdirectory(calc)

# The prefix form calculator.
add_subdirectory(calc2sexpr)

# The postfix form calculator.
add_subdirectory(calc2post)

# Tests.
add_subdirectory(tests)
## Lexer test.
add_test(test-lex tests/lex ${CMAKE_SOURCE_DIR}/tests/inputs/lex.txt)
## Parser test.
add_test(test-parse tests/parse ${CMAKE_SOURCE_DIR}/tests/inputs/parse.txt)
## Eval test.
add_test(test-eval tests/eval ${CMAKE_SOURCE_DIR}/tests/inputs/eval.txt)
## Prefix test.
add_test(test-prefix tests/prefix ${CMAKE_SOURCE_DIR}/tests/inputs/prefix.txt)
## Postfix test.
add_test(test-postfix tests/postfix ${CMAKE_SOURCE_DIR}/tests/inputs/postfix.txt)