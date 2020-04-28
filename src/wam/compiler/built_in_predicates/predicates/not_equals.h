//
// Created by leonhard on 28.04.20.
//

#ifndef PROLOG_BFS_NOT_EQUALS_H
#define PROLOG_BFS_NOT_EQUALS_H

#include <wam/compiler/util/node.h>
#include <wam/executor/executor.h>

namespace wam{
    namespace preds{
        node not_equals_node_tree();
        void not_equals_check(wam::executor& exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);
    }
}
#endif //PROLOG_BFS_NOT_EQUALS_H
