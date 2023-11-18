cmake_minimum_required(VERSION 3.25)
project(PriorityQueue)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g -Wall) #  -O2)

set(EXECUTABLE_OUTPUT_PATH ${EXEC_DIR}/PriorityQueue)

add_executable(BinomialQueue    BinomialQueue.cc     )
add_executable(LeftistHeap      LeftistHeap.cc       )
add_executable(PriorityQueue    PriorityQueue.cc     )
add_executable(PriorityQueueExa PriorityQueueExample.cc)
