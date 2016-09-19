# The calculator executable.
add_executable(calc main.cpp)

# Calculator library.
target_link_libraries(calc libcalc)