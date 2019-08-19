A simple prolog breadth first search interpreter
================================================


Install Instructions
--------------------
Boost.Program_options is required to build this project.
Cmake version VERSION 3.12 is required.

```shell
sudo apt-get install libboost-program-options-dev 
git clone https://github.com/LhKipp/Prolog_BFS.git
cmake CMakeLists.txt
make prolog_bfs
```

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
