#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-03-22 17:13:54
# Last Modified: 2024-10-07 05:54:23
#***********************************************
path=build
rm -rf ${path} ThreadPool lib  bin
sync
cmake -S . -B ${path}
cmake --build ${path} --config Release -j$(nproc)
echo
echo
./bin/ThreadPool

