cmake_minimum_required(VERSION 3.25)
project(Tree)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/Tree)

add_executable( AvlTree           AvlTree.cc         )
add_executable( BinarySearchTree  BinarySearchTree.cc)
#add_executable( example           example.cc         )
