//
// Created by leonhard on 06.03.20.
//
#include "../wam/compiler/parser/parser_error.h"
#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(parser_error) {
        class_<wam::parser_error>("ParserError")
                .constructor()
                .function("get_row", &wam::parser_error::get_row)
                .function("get_col", &wam::parser_error::get_col)
                .function("exists", &wam::parser_error::exists)
                .function("get_expected", &wam::parser_error::get_expected)
        ;
}
