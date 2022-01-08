file(GLOB_RECURSE wasm_sources CONFIGURE_DEPENDS "*.cpp")

add_executable(prolog_bfs_wasm ${wasm_sources})

target_link_libraries(prolog_bfs_wasm PRIVATE prolog_bfs_lib)
# output name should always be prolog_bfs for wasm, so the .js file is always called the same way
set_target_properties(prolog_bfs_wasm PROPERTIES OUTPUT_NAME "prolog_bfs")

# compile options
target_compile_options(prolog_bfs_wasm PUBLIC "SHELL:-s DISABLE_EXCEPTION_CATCHING=0")

# link options
target_link_options(prolog_bfs_wasm PUBLIC --bind)
target_link_options(prolog_bfs_wasm PUBLIC --emrun)
target_link_options(prolog_bfs_wasm PUBLIC "SHELL:-s DISABLE_EXCEPTION_CATCHING=0")
target_link_options(prolog_bfs_wasm PUBLIC "SHELL:-s MODULARIZE=1" "SHELL:-s EXPORT_NAME=\"'EmscriptenModule'\"")
target_link_options(prolog_bfs_wasm PUBLIC "SHELL:-s ALLOW_MEMORY_GROWTH=1" "SHELL:-s MAXIMUM_MEMORY=500MB")
