//
// Created by leonhard on 06.03.20.
//
#include <emscripten/bind.h>
#include <wam/compiler/error/compiler_error.h>

using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(error) {
        class_<compiler::error>("ParserError")
                .constructor()
                .function("get_row", &compiler::error::get_row)
                .function("get_col", &compiler::error::get_col)
                .function("exists", &compiler::error::exists)
                .function("get_expected", &compiler::error::get_description)
        ;
}
