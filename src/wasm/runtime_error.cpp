//
// Created by Martin Weber 2020-05-09
//
#include <wam/bfs_organizer/data/error/runtime_error.h>
#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(runtime_error) {
        class_<wam::runtime_error>("runtime_error")
                //.constructor()
                .function("getTypeAsString", &wam::runtime_error::get_type_as_str)
                .function("getErrorLine", &wam::runtime_error::get_err_line)
                .function("getErrorAtomAsString", &wam::runtime_error::get_err_atom_as_str)
                .function("getExplanation", &wam::runtime_error::get_explanation)
        ;
}
