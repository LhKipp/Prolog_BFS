//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_PROGRAM_TERM_CODE_H
#define PROLOG_BFS_PROGRAM_TERM_CODE_H

#include "../compiler/util/reg_func_counts.h"
#include "var_reg_substitution.h"
#include <functional>
#include <vector>


namespace wam {
    struct executor;

    struct term_code {
    private:
        //For building a nice tree representation, we need to save a visual representation
        //of what this term_code is solving
        //node solves_node?

        //We need to keep track which var_reg_substs is from an original user entered query
        //we store it in term_code to save some heap space
        bool is_from_orig_query;
        std::vector<var_reg_substitution> substitutions;
    public:
        int expected_register_count;
        std::vector<std::function<void(wam::executor &)>> instructions;

        term_code(int expectedRegisterCount,
                  std::vector<std::function<void(executor &)>> instructions,
                  std::vector<var_reg_substitution> substitutions,
                  bool is_from_original_query = false)
                : expected_register_count(expectedRegisterCount),
                  instructions(std::move(instructions)),
                  substitutions(std::move(substitutions)),
                  is_from_orig_query(is_from_original_query) {
        }

        term_code() = default;

        inline bool is_from_original_query() const {
            return is_from_orig_query;
        }

        const std::vector<var_reg_substitution> &get_substitutions() const {
            return substitutions;
        }
    };
}
#endif //PROLOG_BFS_PROGRAM_TERM_CODE_H
