//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_UNDEFINED_VAR_H
#define PROLOG_BFS_UNDEFINED_VAR_H


#include <wam/compiler/util/node.h>
#include <unordered_map>
#include <wam/compiler/util/seen_register.h>

namespace compiler{
    void check_and_throw_undefined_var(
            const node& atom,
            const node& expr,
            const std::unordered_map<wam::helper::seen_register, bool> &seen_registers);
}

#endif //PROLOG_BFS_UNDEFINED_VAR_H
