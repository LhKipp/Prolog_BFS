A simple prolog breadth first search interpreter
================================================


Install Instructions
--------------------
Boost.Program_options is required to build this project.
Cmake version VERSION 3.10 is required.

```shell
sudo apt-get install libboost-program-options-dev 
git clone https://github.com/LhKipp/Prolog_BFS.git
cd Prolog_BFS
./build_xxx.sh
```
There is 2 different build scripts. "build_native" builds the cli application and "build_wasm" for WebAssembly. Please note that you must delete the entire CMakeFiles folder and CMakeCache.txt prior to using a different build script version. Otherwise the build will fail.

Known Limitations
-----------------
1. There is no syntax checker for the program. Passing syntacticaly wrong code to the bfs-interpreter, may cause undefined behaviour.
        It is recommended to let your code be checked by an external tool before passing it to the bfs-interpreter.
2. Any kind of arithmetic is not supported. Use o for 0, s(o) for 1, s(s(o)) for 2, ...
3. The underscore in variable / constant names is not supported.
4. There are no built-in predicates.


How to use it
-------------
Suppose your executable is named "prolog_bfs".
Type "prolog_bfs program_file" and it will run.

WebAssembly: run "start_server.sh" and go to http://localhost:8080/
