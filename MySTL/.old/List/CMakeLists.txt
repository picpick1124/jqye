cmake_minimum_required(VERSION 3.25)
project(List)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/List)
add_definitions(-DTEST_VECTOR)
add_definitions(-DTEST_LIST)
add_definitions(-DTEST_STACK)


add_executable( test   test.cc  )
