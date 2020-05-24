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
                .function("getType", &wam::runtime_error::get_err_type)
                .function("getErrorLine", &wam::runtime_error::get_err_line)
                .function("getErrorAtomAsString", &wam::runtime_error::get_err_atom_as_str)
                .function("getExplanation", &wam::runtime_error::get_explanation)
        ;
}

EMSCRIPTEN_BINDINGS(ERROR_TYPE) {
    enum_<ERROR_TYPE>("ERROR_TYPE")
        .value("OUT_OF_TIME", ERROR_TYPE::OUT_OF_TIME)
        .value("NONE", ERROR_TYPE::NONE)
        .value("OUT_OF_MEMORY", ERROR_TYPE::OUT_OF_MEMORY)
        .value("DIV_BY_0", ERROR_TYPE::DIV_BY_0)
        .value("ARGUMENTS_NOT_SUFF_INSTANCIATED", ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED)

        .value("FUNCTOR_IS_NOT_ARITHMETIC", ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC)
        ;
}
