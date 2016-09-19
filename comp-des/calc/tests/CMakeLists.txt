add_executable(lex lex.cpp)
target_link_libraries(lex libcalc)

add_executable(parse parse.cpp)
target_link_libraries(parse libcalc)

add_executable(eval eval.cpp)
target_link_libraries(eval libcalc)

add_executable(prefix prefix.cpp)
target_link_libraries(prefix libcalc)

add_executable(postfix postfix.cpp)
target_link_libraries(postfix libcalc)