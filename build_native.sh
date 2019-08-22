#!/bin/sh
# 
# File:   build_native.sh
# Author: martin
#
# Created on Aug 22, 2019, 4:39:14 PM
#

# the filename is hardcoded in cmake so we need to do
# some ugly hacks here
cp CMakeLists.txt.native CMakeLists.txt

rm build -r
mkdir build
cd build
cmake ..
make prolog_bfs
