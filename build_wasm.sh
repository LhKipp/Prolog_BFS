#!/bin/bash

# the filename is hardcoded in cmake so we need to do
# some ugly hacks here
# TODO Make 1 CMake file and pass and option to build either wasm or normal
cp CMakeLists.txt.wasm CMakeLists.txt

# Be sure to have sourced the emsdk paths !!!
emcmake cmake CMakeLists.txt
emmake make prolog_bfs

# link the build output to the public directory so it can be accessed despite
# not being in the document root of the server
ln -sf ../build web/build
