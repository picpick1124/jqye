
make_minimum_required(VERSION 3.25)
project(List)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/List)

add_executable( List   List.cc  )
add_executable( Queue  Queue.cc )
add_executable( Stack0 Stack0.cc)
add_executable( Stack1 Stack1.cc)
add_executable( Stack2 Stack2.cc)
add_executable( Vector Vector.cc)
