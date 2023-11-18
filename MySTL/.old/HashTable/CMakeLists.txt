cmake_minimum_required(VERSION 3.25)
project(HashTable)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)


set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/HashTable)

#include_directories(.)
add_executable( CukooHashTable          CukooHashTable.cc          )
#add_executable( OpenAddressingHashtable OpenAddressingHashtable.cc )
add_executable( HashTable               HashTable.cc               )
