project(eigen_decomp CXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8")
    message(FATAL_ERROR "Requires GCC version 4.8 or greater for C++11 features")
    endif()
else()
    message(FATAL_ERROR "Requires a GNU C++ Compiler")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O2")

add_executable(eigen_decomp 
  mat.cpp
  main.cpp)
