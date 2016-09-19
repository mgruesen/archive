add_executable(lex lex.cpp)
target_link_libraries(lex libcalc)

add_executable(parse parse.cpp)
target_link_libraries(parse libcalc)

add_executable(eval eval.cpp)
target_link_libraries(eval libcalc)