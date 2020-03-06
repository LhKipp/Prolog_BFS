//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_BASE_GRAMMAR_H
#define PROLOG_BFS_BASE_GRAMMAR_H

#include "parser_error.h"

namespace wam{
    template <typename=void> struct error_handler {
    error_handler() = default;
    error_handler(error_handler const&) = delete;

    template<typename...> struct result { typedef void type; };

    template<typename Iter, typename String>
    void operator()(parser_error& err, Iter begin, Iter end, Iter error_pos, String what) const {
        err.set_exists(true);
        err.set_row(std::count(begin, error_pos, '\n'));
        err.set_col(std::distance(
                std::find(std::make_reverse_iterator(error_pos),
                        std::make_reverse_iterator(begin),
                        '\n').base(),
                        error_pos
                ));
        std::stringstream ss{};
        ss << what;
        err.set_expected(ss.str());
    }
//    mutable int proof = 0;
};
}
#endif //PROLOG_BFS_BASE_GRAMMAR_H
