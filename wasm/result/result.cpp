//
// Created by Martin Weber 2020-05-09
//
#include <prolog_bfs/wam/bfs_organizer/data/result.h>
#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(result) {
        class_<wam::result>("result")
                //.constructor()
                .function("getRuntimeStatistics", &wam::result::get_statistics)
                .function("isError", &wam::result::is_error)
                .function("isAnswer", &wam::result::is_answer)
                .function("getError", &wam::result::get_error)
                .function("getAnswerAsString", &wam::result::get_answer_as_str)
        ;
}
