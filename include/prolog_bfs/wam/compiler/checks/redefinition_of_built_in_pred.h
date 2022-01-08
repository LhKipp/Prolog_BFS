//
// Created by leonhard on 16.05.20.
//

#ifndef PROLOG_BFS_REDEFINITION_OF_BUILT_IN_PRED_H
#define PROLOG_BFS_REDEFINITION_OF_BUILT_IN_PRED_H


#include "prolog_bfs/wam/executor/executor.h"

namespace compiler{
    void check_and_throw_redefinition_of_built_in_pred(const node& head_func);
}

#endif //PROLOG_BFS_REDEFINITION_OF_BUILT_IN_PRED_H
