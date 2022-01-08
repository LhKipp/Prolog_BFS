#!/bin/sh

TEMP=$(getopt -o rw --long release,wasm -- "$@")

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

# Note the quotes around '$TEMP': they are essential!
eval set -- "$TEMP"

RELEASE=false
WASM=false
while true; do
  case "$1" in
    -r | --release ) RELEASE=true; shift ;;
    -w | --wasm ) WASM=true; shift ;;
    -- ) shift; break ;;
    * ) break ;;
  esac
done

OPTS=""
TARGET_DIR="build"
if [ $WASM == "true" ]; then
    OPTS="${OPTS} -DWASM_BUILD=ON"
    TARGET_DIR="wasm_out"
fi
if [ $RELEASE == "true" ]; then
    OPTS="${OPTS} -DCMAKE_BUILD_TYPE=Release"
    TARGET_DIR="${TARGET_DIR}_release"
fi

if [ ! -d "$TARGET_DIR" ]; then
    mkdir "$TARGET_DIR"
fi

pushd "$TARGET_DIR"
if [ $WASM == "true" ]; then
    # Not quoting $OPTS so it can expand to multiple options
    echo "emcmake cmake .. $OPTS && emmake make -j $(($(nproc) + 1))"
    emcmake cmake .. $OPTS && emmake make -j $(($(nproc) + 1))
    popd

    # link the build output to the public directory so it can be accessed despite
    # not being in the document root of the server
    echo "Linking web to $TARGET_DIR"
    ln -sfn "../$TARGET_DIR" web/build
    ln -sf wasm/prolog_bfs.js "$TARGET_DIR"/prolog_bfs.js
    ln -sf wasm/prolog_bfs.wasm "$TARGET_DIR"/prolog_bfs.wasm

    # TODO this is because of a bug reported here: https://github.com/emscripten-core/emscripten/issues/9637#issuecomment-632742739
    ln -sf ../"$TARGET_DIR"/prolog_bfs.data web/prolog_bfs.data
else
    echo "cmake .. $OPTS && make -j $(($(nproc) + 1))"
    cmake .. $OPTS && make -j $(($(nproc) + 1))
    popd
fi

