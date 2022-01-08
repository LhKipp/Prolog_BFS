//
// Created by leonhard on 02.08.19.
//

#ifndef PROLOG_BFS_ENVIRONMENT_H
#define PROLOG_BFS_ENVIRONMENT_H

#include "prolog_bfs/wam/data/heap_reg.h"
#include <vector>
namespace wam {
    struct environment {
        std::vector <wam::heap_reg> permanent_registers;

        environment() = default;
        environment(size_t permanent_var_count): permanent_registers{permanent_var_count}{}
    };
}

#endif //PROLOG_BFS_ENVIRONMENT_H
