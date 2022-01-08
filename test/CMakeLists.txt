# Testing library
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS "*.cpp" "*.h")

add_executable(prolog_bfs_test ${test_sources})

# To include test_util.h
target_include_directories(prolog_bfs_test PRIVATE .)

target_compile_features(prolog_bfs_test PRIVATE cxx_std_17)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(prolog_bfs_test PRIVATE prolog_bfs_lib Catch2::Catch2)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
# add_test(NAME testlibtest COMMAND testlib) # Command can be a target
