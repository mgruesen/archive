project(pa1 CXX)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")

set(strops-src ../util/strops.cpp)
set(tree-src ../util/tree.cpp)

add_library(util STATIC
    ${strops-src}
    ${tree-src})

add_executable(lex lexer.cpp)
add_executable(compile compiler.cpp)
add_executable(codegen codegen.cpp)

target_link_libraries(lex util)
target_link_libraries(compile util)
target_link_libraries(codegen util)