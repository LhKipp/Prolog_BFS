cmake_minimum_required(VERSION 3.13)

project(PrologBfs VERSION 1.0
                  DESCRIPTION "A prolog bfs compiler"
                  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# set where the build output should go
find_package(Boost 1.60 COMPONENTS program_options REQUIRED )

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(test)

#make include from src possible
# target_include_directories(prolog_bfs PRIVATE ${PROJECT_SOURCE_DIR}/src)
