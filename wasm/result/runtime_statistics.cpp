//
// Created by leonhard on 24.05.20.
//
#include <prolog_bfs/wam/bfs_organizer/data/runtime_statistics.h>
#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(runtime_statistics) {
        class_<wam::runtime_statistics>("runtime_statistics")
                //.constructor()
                .function("getQueriesAskedCount", &wam::runtime_statistics::get_queries_asked_count)
        ;
}
