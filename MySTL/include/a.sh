#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-07-07 20:32:27
# Last Modified: 2024-09-18 17:55:28
#***********************************************

rm -rf build bin bbb TAGS
cmake -S . -B build
echo
echo
cmake --build build  -j$(nproc)

echo

read -p "run/gdb/xclip/no [r/g/x/n] "
case $REPLY in
	r) ./bin/test
		;;
	g) gdb ./bin/test
		;;
	x) ./bin/test | xclip -sel clip
		;;
esac

