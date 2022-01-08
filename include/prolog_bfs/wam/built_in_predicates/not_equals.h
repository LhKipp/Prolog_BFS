//
// Created by leonhard on 28.04.20.
//

#ifndef PROLOG_BFS_NOT_EQUALS_H
#define PROLOG_BFS_NOT_EQUALS_H

#include "prolog_bfs/wam/compiler/util/node.h"
#include "prolog_bfs/wam/executor/executor.h"

namespace wam{
    namespace preds{
        void not_equals_check(wam::executor& exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);
    }
}
#endif //PROLOG_BFS_NOT_EQUALS_H
