A simple prolog breadth first search interpreter
================================================


How to use it
-------------
Visit: https://askuri.github.io/PrologBFS/wasm/
or build it from source.
Suppose your executable is named "prolog_bfs",
type "prolog_bfs program_file" and it will run.
WebAssembly: run "start_server.sh" and go to http://localhost:8080/

Install Instructions
--------------------
# Prerequisites
Boost.Program_options, Boost.Spirit, Boost.Phoenix
Cmake version 3.10 or higher
Compiler for C++ 17 or higher 
Optional: Emsdk for building wasm files.

```shell
#Boost libraries
sudo apt-get install libboost-all-dev
git clone https://github.com/LhKipp/Prolog_BFS.git
cd Prolog_BFS
./build_xxx.sh
```
There are 2 different build scripts. "build_native" builds the cli application and "build_wasm" builds the WebAssembly files.
To build the wasm version further steps are required. Make sure to check the CMakeList.txt.wasm for further instructions.

Known Limitations
-----------------
1. Any kind of arithmetic is not supported. Use o for 0, s(o) for 1, s(s(o)) for 2, ...
2. Variables / constant names starting with an underscore are not supported. (This implies that a single "_" as a variable name is not supported)
3. There are no built-in predicates.
4. The online version has a limit of 500MB memory. For the offline version there is no memory usage check. If an endless program has been supplied, the program won't stop allocating memory. Make sure to kill the program before all memory is exhausted.



Contributors
------------
- Martin Weber (https://github.com/askuri). Created the web version, tested the program, gave constructive feedback, found some bugs. Big thanks.
