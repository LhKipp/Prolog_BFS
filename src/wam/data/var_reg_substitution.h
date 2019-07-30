#include <utility>

//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_H
#define PROLOG_BFS_SUBSTITUTION_H

namespace wam {
    struct substitution {
        std::string var_name;
        size_t register_index;

        substitution() = default;

        substitution(std::string varName, size_t registerIndex) : var_name(std::move(varName)),
                                                                         register_index(registerIndex) {}
    };
}
#endif //PROLOG_BFS_SUBSTITUTION_H
