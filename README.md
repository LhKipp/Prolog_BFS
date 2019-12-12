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
Boost.Program_options is required to build this project.
Cmake version VERSION 3.10 is required.

Optional: Emsdk for building wasm files.

```shell
sudo apt-get install libboost-program-options-dev 
git clone https://github.com/LhKipp/Prolog_BFS.git
cd Prolog_BFS
./build_xxx.sh
```
There are 2 different build scripts. "build_native" builds the cli application and "build_wasm" builds the WebAssembly files.

Known Limitations
-----------------
1. There is no syntax checker for the program. Passing syntacticaly wrong code to the bfs-interpreter, may cause undefined behaviour.
        It is recommended to let your code be checked by an external tool before passing it to the bfs-interpreter.
        Note: If you are using the webversion, check out the developer console to see whether any exceptions have been thrown.
2. Any kind of arithmetic is not supported. Use o for 0, s(o) for 1, s(s(o)) for 2, ...
3. Variables / constant names starting with an underscore are not supported.
4. There are no built-in predicates.
5. The online version has a limit of 500MB memory. For the offline version there is no memory usage check. If an endless program has been supplied, the program won't stop allocating memory. Make sure to kill the program before all memory is exhausted.


Contributors
------------
- Martin Weber (https://github.com/askuri). Created the web version, tested the program, gave constructive feedback, found some bugs . Big thanks.
