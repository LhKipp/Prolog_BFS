//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_INSTRUCTIONS_UTIL_H
#define PROLOG_BFS_INSTRUCTIONS_UTIL_H

#include "wam/data/heap_reg.h"
#include "../../executor/executor.h"

namespace wam{
    size_t deref(const executor& exec,const heap_reg& init_regist);
    size_t deref(const executor& exec, size_t heap_i);
    heap_reg derefed_reg(const executor& exec, size_t heap_i);
    heap_reg derefed_reg(const executor& exec, const heap_reg& init_regist);
    heap_reg& derefed_reg_modify(executor &exec, size_t heap_i);
    heap_reg& derefed_reg_modify(executor &exec, const heap_reg &init_regist);
}
#endif //PROLOG_BFS_INSTRUCTIONS_UTIL_H
