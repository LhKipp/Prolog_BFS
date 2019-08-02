//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_H
#define PROLOG_BFS_SUBSTITUTION_H

#include <string>

namespace wam {
    struct var_reg_substitution {
        std::string var_name;
        size_t register_index;
        bool is_permanent_register;

        var_reg_substitution() = default;

        var_reg_substitution(std::string varName, size_t registerIndex, bool permanent_regist) : var_name( std::move(varName)),
                                                                                                 register_index( registerIndex),
                                                                                                 is_permanent_register{ permanent_regist} {}
    };
}
#endif //PROLOG_BFS_SUBSTITUTION_H
