//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_PROGRAM_TERM_CODE_H
#define PROLOG_BFS_PROGRAM_TERM_CODE_H

#include "../parser/util/reg_func_counts.h"
#include <functional>
#include <vector>


namespace wam {
    struct executor;

    struct term_code{
        helper::reg_func_counts expected_register_count;
        functor_view term_functor_name;
        std::vector<std::function<void(wam::executor&)>> instructions;

        term_code() = default;
        term_code(helper::reg_func_counts expected_register_count,
                            functor_view term_functor_name,
                            std::vector<std::function<void(wam::executor&)>> instructions):
                                expected_register_count{expected_register_count},
                                term_functor_name{std::move(term_functor_name)},
                                instructions{std::move(instructions)}{}

    };
}
#endif //PROLOG_BFS_PROGRAM_TERM_CODE_H
