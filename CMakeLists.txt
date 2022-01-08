cmake_minimum_required(VERSION 3.13)

project(prolog_bfs VERSION 1.0
                   DESCRIPTION "A prolog bfs compiler"
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# set where the build output should go
find_package(Boost 1.60 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

file(GLOB_RECURSE prolog_bfs_src
    "src/*.h"
    "src/*.cpp"
)
list(FILTER prolog_bfs_src EXCLUDE REGEX ".*wasm/.*")

file(GLOB_RECURSE prolog_bfs_test
        "test/*.h"
        "test/*.cpp"
        )
add_executable(prolog_bfs main.cpp catch.hpp ${prolog_bfs_src} ${prolog_bfs_test})

#make include from src possible
target_include_directories(prolog_bfs PRIVATE ${PROJECT_SOURCE_DIR}/src)

TARGET_LINK_LIBRARIES( prolog_bfs LINK_PUBLIC ${Boost_LIBRARIES} )
