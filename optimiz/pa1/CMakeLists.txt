project(pa1 CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")

add_executable(lex 
    lexer.cpp
    strops.cpp)
add_executable(compile 
    compiler.cpp
    strops.cpp)
