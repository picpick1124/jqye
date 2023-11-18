#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-03-22 17:13:54
# Last Modified: 2024-06-03 22:59:22
#***********************************************
path=build
cmake -S . -B ${path}
cmake --build ${path} --config Release -j12
echo
echo
