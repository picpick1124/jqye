#!/bin/sh

rm -rf build bin bbb TAGS
cmake -S . -B build
echo
echo
cmake --build build  -j12
