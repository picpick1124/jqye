cmake_minimum_required(VERSION 3.25)
project(SomeAlgorithm)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/SomeAlgorithm)

add_executable( BinarySearch BinarySearch.cc)
add_executable( example      example.cc     )
add_executable( gcd          gcd.cc         )
add_executable( pow          pow.cc         )
