//
// Created by leonhard on 27.04.20.
//

#ifndef PROLOG_BFS_EQUALS_H
#define PROLOG_BFS_EQUALS_H

#include <wam/compiler/util/node.h>
#include <wam/executor/executor.h>

namespace wam{
    namespace preds{
        node equals_node_tree();
        void equals_check(wam::executor& exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);

        bool heap_reg_equals(wam::executor& exec, size_t lhs, size_t rhs);
    }
}


#endif //PROLOG_BFS_EQUALS_H
