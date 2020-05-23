//
// Created by leonhard on 09.03.20.
//

#ifndef PROLOG_BFS_ERROR_HANDLER_H
#define PROLOG_BFS_ERROR_HANDLER_H

#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <wam/compiler/error/compiler_error.h>

namespace wam {
    template<typename=void>
    struct error_handler {
        error_handler() = default;

        error_handler(error_handler const &) = delete;

        template<typename...>
        struct result {
            typedef void type;
        };

        template<typename Iter, typename String>
        void operator()(compiler::error &err, Iter begin, Iter end, Iter error_pos, String what) const {
            err.type = compiler::ERROR_TYPE::PARSER_ERROR;
            err.source_info.line_begin = boost::spirit::get_line(error_pos);
            err.source_info.line_end = boost::spirit::get_line(error_pos);
            err.column = boost::spirit::get_column(begin, error_pos);
            std::stringstream ss{};
            ss << what;
            err.description = ss.str();
        }
    };
}
#endif //PROLOG_BFS_ERROR_HANDLER_H
