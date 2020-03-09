//
// Created by leonhard on 09.03.20.
//

#ifndef PROLOG_BFS_ERROR_HANDLER_H
#define PROLOG_BFS_ERROR_HANDLER_H

#include <algorithm>
#include "../parser_error.h"
#include <boost/spirit/include/support_line_pos_iterator.hpp>
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
        void operator()(parser_error &err, Iter begin, Iter end, Iter error_pos, String what) const {
            err.set_exists(true);
            //row and col are not 0 based in boost spirit, thats why -1
            err.set_row(boost::spirit::get_line(error_pos) -1);
            err.set_col(boost::spirit::get_column(begin, error_pos) -1);
            std::stringstream ss{};
            ss << what;
            err.set_expected(ss.str());
        }
    };
}
#endif //PROLOG_BFS_ERROR_HANDLER_H
