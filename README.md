A simple prolog breadth first search interpreter
================================================


# How to use it
Visit: https://askuri.github.io/PrologBFS/wasm/
or build it from source.
Suppose your executable is named "prolog_bfs",
type "prolog_bfs program_file" and it will run.
WebAssembly: run "start_server.sh" and go to http://localhost:8080/

# Install Instructions
## Prerequisites
Dependencies:

* Boost.Program_options, Boost.Spirit, Boost.Phoenix
* Cmake version 3.13 or higher
* Compiler for C++ 17 or higher
* Optional: Emsdk for building wasm files, version 1.39.10 or higher: https://emscripten.org/docs/getting_started/downloads.html#installation-instructions

We develop on Ubuntu Linux. If you use windows or another OS, you might need to change
the commands accordingly.

## Install
```shell
#Boost libraries
sudo apt-get install libboost-all-dev
git clone https://github.com/LhKipp/Prolog_BFS.git
cd Prolog_BFS
./build_xxx.sh
```
There are 2 different build scripts. "build_native" builds the cli application and "build_wasm" builds the WebAssembly files.
If you first build wasm, then native (or the other way around) make sure to delete the `CMakeCache.txt` file! Otherwise the build might fail.

## Building native
Run `build_native.sh`. This runs currently only test cases.

## Building for WebAssembly
1. Source your emsdk environment
2. Run `./build_wasm.sh <env>` where <env> can be either
    * `dev` for a development build (fast compilation, no optimization, no test suite)
    * `prod` for a production build (slow compilation, O3 optimization, no test suite) 
    * `test` for a testing build (even slower compilation, O3 optimization, with test suite) 
You may omit <env>, resulting in a dev build.

### Running tests in the browser
1. Run a test build
2. Open your browser's javascript console while being on the prolog bfs site
3. Run `runTests()` in the console. You should see the test output there

# Known Limitations
1. Only integer arithmetic is supported.
2. Variables / constant names starting with an underscore are not supported. (This implies that a single "_" as a variable name is not supported)
3. The only supported built in predicates are ==, \==, is/2 and append/3.
4. The online version has a limit of 500MB memory. For the offline version there is no memory usage check. If an endless program has been supplied, the program won't stop allocating memory. Make sure to kill the program before all memory is exhausted.



# Contributors
- Martin Weber (https://github.com/askuri). Created the web frontend, tested the program, created documentation, updated the build scripts, gave constructive feedback, found some bugs. Big thanks.
