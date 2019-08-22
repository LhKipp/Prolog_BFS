#!/bin/bash

# the filename is hardcoded in cmake so we need to do
# some ugly hacks here
cp CMakeLists.txt.wasm CMakeLists.txt

# prepare the environment for emsdk
# SET EMSDK PATH HERE
source ~/Apps/emsdk/emsdk_env.sh

emconfigure cmake CMakeLists.txt
emmake make prolog_bfs
