//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_EXPR_EVALUATOR_H
#define PROLOG_BFS_EXPR_EVALUATOR_H

#include <prolog_bfs/wam/executor/executor.h>

namespace wam::arithmetic{
    /*
     * Evaluates the given heap_reg and returns:
     *  heap_reg of type int
     *  heap_reg of type double TODO not supported yet
     * depending on the value of the expr
     */
    wam::heap_reg eval_arithmetic_reg(executor& exec, size_t rhs_heap_i);
    wam::heap_reg eval_arithmetic_func(executor& exec,
                                       heap_reg& func,
                                       size_t func_heap_i);

}

#endif //PROLOG_BFS_EXPR_EVALUATOR_H
