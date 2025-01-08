#!/bin/sh
#***********************************************
#
#      Filename: a.sh
#
#        Author: aeon - meidanzo@foxmail
#   Description:  
#        Create: 2024-06-01 04:10:48
# Last Modified: 2024-09-20 21:48:18
#***********************************************

while (( 1 )) ; do
	echo
	echo
if [[  -f core ]];then
	return
fi
./build.sh
done
