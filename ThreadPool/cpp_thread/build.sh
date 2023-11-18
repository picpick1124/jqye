#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-03-22 17:13:54
# Last Modified: 2024-06-01 15:54:55
#***********************************************
path=build
rm -rf ${path} ThreadPool lib  bin
cmake -S . -B ${path}
cmake --build ${path} --config Release -j12
echo
echo
./bin/ThreadPool

