cmake_minimum_required(VERSION 3.13)

project(PrologBfs VERSION 1.0
                  DESCRIPTION "A prolog bfs compiler"
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# set where the build output should go
if(NOT WASM_BUILD)
    # WASM_BUILD includes by setting compiler option
    find_package(Boost 1.60 COMPONENTS program_options REQUIRED )
endif()

option(WASM_BUILD "True if compiling for wasm" OFF)

add_subdirectory(src)
if(WASM_BUILD)
    add_subdirectory(wasm)
else()
    add_subdirectory(apps)
    add_subdirectory(test)
endif()
