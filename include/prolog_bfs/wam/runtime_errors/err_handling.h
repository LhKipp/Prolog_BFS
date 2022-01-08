//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_ERR_HANDLING_H
#define PROLOG_BFS_ERR_HANDLING_H

#include "prolog_bfs/wam/bfs_organizer/data/error/runtime_error.h"
#include "prolog_bfs/wam/executor/executor.h"

namespace err_handling{

    void check_and_throw_is_evaluable(wam::executor& exec, wam::heap_reg reg);
}


#endif //PROLOG_BFS_ERR_HANDLING_H
