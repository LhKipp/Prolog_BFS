//
// Created by leonhard on 02.08.19.
//

#ifndef PROLOG_BFS_ENVIRONMENT_H
#define PROLOG_BFS_ENVIRONMENT_H

#include "heap_reg_with_i.h"

namespace wam {
    struct environment {
        std::vector <wam::heap_reg_with_i> permanent_registers;

        environment() = default;
        environment(size_t permanent_var_count): permanent_registers{permanent_var_count}{}
    };
}

#endif //PROLOG_BFS_ENVIRONMENT_H
