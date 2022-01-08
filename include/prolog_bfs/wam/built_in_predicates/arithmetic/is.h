//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_IS_H
#define PROLOG_BFS_IS_H

#include <wam/compiler/util/node.h>
#include <wam/executor/executor.h>
#include <wam/compiler/util/seen_register.h>
#include <wam/bfs_organizer/data/storage.h>
#include <wam/instructions/instructions.h>

namespace wam::preds {
    void is(wam::executor &exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);
}

#endif //PROLOG_BFS_IS_H
