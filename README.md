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
./build.sh
```

## Building
Run `./build.sh --help` for help on how to build the program

# Known Limitations
1. Only integer arithmetic is supported.
2. Variables / constant names starting with an underscore are not supported. (This implies that a single "_" as a variable name is not supported)
3. The only supported built in predicates are ==, \==, is/2 and append/3.
4. The online version has a limit of 500MB memory. For the offline version there is no memory usage check. If an endless program has been supplied, the program won't stop allocating memory. Make sure to kill the program before all memory is exhausted.



# Contributors
- Martin Weber (https://github.com/askuri). Created the web frontend, tested the program, created documentation, updated the build scripts, gave constructive feedback, found some bugs. Big thanks.
