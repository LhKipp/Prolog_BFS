//
// Created by leonhard on 27.04.20.
//

#ifndef PROLOG_BFS_EQUALS_H
#define PROLOG_BFS_EQUALS_H

#include <prolog_bfs/wam/compiler/util/node.h>
#include <prolog_bfs/wam/executor/executor.h>

namespace wam{
    namespace preds{
        void equals_check(wam::executor& exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);

        bool heap_reg_equals(const wam::executor& exec, size_t lhs, size_t rhs);

        bool functor_equals(const wam::executor& exec, int arity, size_t lhs, size_t rhs);

    }
}


#endif //PROLOG_BFS_EQUALS_H
