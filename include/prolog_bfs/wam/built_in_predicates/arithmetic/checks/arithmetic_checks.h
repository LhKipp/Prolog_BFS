//
// Created by leonhard on 11.05.20.
//

#ifndef PROLOG_BFS_ARITHMETIC_CHECKS_H
#define PROLOG_BFS_ARITHMETIC_CHECKS_H

#include <prolog_bfs/wam/executor/executor.h>

namespace wam::arithmetic{
    void check_and_throw_if_wrong_values(executor &executor, const heap_reg &func, const heap_reg& lhs, const heap_reg& rhs);

}

#endif //PROLOG_BFS_ARITHMETIC_CHECKS_H
