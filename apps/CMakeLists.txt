add_executable(prolog_bfs main.cpp)
target_compile_features(prolog_bfs PRIVATE cxx_std_17)
target_link_libraries(prolog_bfs PRIVATE Boost::program_options)
target_link_libraries(prolog_bfs PRIVATE prolog_bfs_lib)
