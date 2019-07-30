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

        var_reg_substitution() = default;

        var_reg_substitution(std::string varName, size_t registerIndex) : var_name(std::move(varName)),
                                                                         register_index(registerIndex) {}
    };
}
#endif //PROLOG_BFS_SUBSTITUTION_H
