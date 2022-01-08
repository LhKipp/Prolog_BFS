//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_IS_H
#define PROLOG_BFS_IS_H

#include "prolog_bfs/wam/compiler/util/node.h"
#include "prolog_bfs/wam/executor/executor.h"
#include "prolog_bfs/wam/compiler/util/seen_register.h"
#include "prolog_bfs/wam/bfs_organizer/data/storage.h"
#include "prolog_bfs/wam/instructions/instructions.h"

namespace wam::preds {
    void is(wam::executor &exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i);
}

#endif //PROLOG_BFS_IS_H
