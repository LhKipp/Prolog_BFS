cmake_minimum_required(VERSION 3.12)
project(prolog_bfs)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE(Boost 1.60 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

file(GLOB_RECURSE prolog_bfs_src
    "src/*.h"
    "src/*.cpp"
)
file(GLOB_RECURSE prolog_bfs_test
        "test/*.h"
        "test/*.cpp"
        )
add_executable(prolog_bfs main.cpp catch.hpp ${prolog_bfs_src} ${prolog_bfs_test})

TARGET_LINK_LIBRARIES( prolog_bfs LINK_PUBLIC ${Boost_LIBRARIES} )
