#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-03-22 17:13:54
# Last Modified: 2024-10-07 13:09:08
#***********************************************
path=build
rm -rf ${path} ThreadPool lib  bin
cmake -S . -B ${path}
cmake --build ${path} --config Release -j$(nproc)
echo
echo
./bin/ThreadPool
#./bin/ThreadPool && rm -f  valgrind.txt && valgrind -s  --log-file=./valgrind.txt --leak-check=full --track-origins=yes --show-leak-kinds=all ./bin/ThreadPool


