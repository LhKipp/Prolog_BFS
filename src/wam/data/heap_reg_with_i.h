//
// Created by leonhard on 10.05.20.
//

#ifndef PROLOG_BFS_HEAP_REG_WITH_I_H
#define PROLOG_BFS_HEAP_REG_WITH_I_H

#include "heap_reg.h"

namespace wam{
    struct heap_reg_with_i{
        heap_reg reg;
        size_t heap_i;

        heap_reg_with_i() {}
        heap_reg_with_i(const heap_reg &heapReg, size_t heapI) : reg(heapReg), heap_i(heapI) {}

    };
}

#endif //PROLOG_BFS_HEAP_REG_WITH_I_H
