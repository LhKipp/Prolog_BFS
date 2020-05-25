#!/bin/bash

# the filename is hardcoded in cmake so we need to do
# some ugly hacks here
# TODO Make 1 CMake file and pass and option to build either wasm or normal
cp CMakeLists.txt.wasm CMakeLists.txt

# Be sure to have sourced the emsdk paths !!!
emcmake cmake CMakeLists.txt

case $1 in
    "prod") echo "Running production built"
            emmake make -j $(($(nproc) + 1)) prolog_bfs_prod
        ;;
    ""|"dev") echo "Running development built"
            emmake make -j $(($(nproc) + 1)) prolog_bfs_dev
        ;;
    "test") echo "Running testing built"
            emmake make -j $(($(nproc) + 1)) prolog_bfs_test
        ;;
    *|"help") echo "Allowed options: prod, dev, test. e.g. build_wasm.sh prod"
esac

# link the build output to the public directory so it can be accessed despite
# not being in the document root of the server
ln -sf ../build web/build

# TODO this is because of a bug reported here: https://github.com/emscripten-core/emscripten/issues/9637#issuecomment-632742739
ln -sf ../build/prolog_bfs.data web/prolog_bfs.data
