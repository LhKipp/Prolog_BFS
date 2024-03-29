file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${PrologBfs_SOURCE_DIR}/include/prolog_bfs/*.h")
# list(FILTER prolog_bfs_src EXCLUDE REGEX ".*wasm/.*")
add_library(prolog_bfs_lib ${sources} ${headers})

target_include_directories(prolog_bfs_lib PUBLIC ../include)

if(WASM_BUILD)
    target_compile_options(prolog_bfs_lib PUBLIC "SHELL:-s USE_BOOST_HEADERS=1")
    target_compile_options(prolog_bfs_lib PUBLIC "SHELL:-s DISABLE_EXCEPTION_CATCHING=0")
else()
    target_link_libraries(prolog_bfs_lib PRIVATE Boost::boost)
endif()
target_compile_features(prolog_bfs_lib PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${headers})
