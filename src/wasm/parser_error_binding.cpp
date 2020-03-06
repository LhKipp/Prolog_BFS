//
// Created by leonhard on 06.03.20.
//
#include "../wam/compiler/parser/parser_error.h"
#include <emscripten/bind.h>

// Binding code
EMSCRIPTEN_BINDINGS(parser_error) {
        class_<wam::parser_error>("ParserError")
                .constructor()
                .property("on_row", &wam::parser_error::get_row, &wam::parser_error::set_row)
                .property("on_col", &wam::parser_error::get_col, &wam::parser_error::set_col)
                .property("_exists", &wam::parser_error::exists, &wam::parser_error::set_exists)
                .property("expected", &wam::parser_error::get_expected, &wam::parser_error::set_expected)
        ;
}
