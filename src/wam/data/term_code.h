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

    struct term_code {
    private:
        int original_query_atom_number;
    public:
        helper::reg_func_counts expected_register_count;
        std::vector<std::function<void(wam::executor &)>> instructions;

        term_code(const helper::reg_func_counts &expectedRegisterCount,
                  std::vector<std::function<void(executor &)>> instructions,
                  int original_query_atom_number = std::numeric_limits<int>::max())
                : expected_register_count(expectedRegisterCount),
                  instructions(std::move(instructions)), original_query_atom_number(original_query_atom_number) {}

        term_code() = default;

        inline bool is_from_original_query() const {
            return original_query_atom_number != std::numeric_limits<int>::max();
        }

        inline int get_atom_number() const {
            return original_query_atom_number;
        }
    };
}
#endif //PROLOG_BFS_PROGRAM_TERM_CODE_H
