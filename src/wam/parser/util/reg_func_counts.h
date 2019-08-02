//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_REG_FUNC_COUNTS_H
#define PROLOG_BFS_REG_FUNC_COUNTS_H

#include <cstddef>

namespace wam::helper {
    struct reg_func_counts {

        size_t x_a_regs_counts; //== needed x_regs + needed a_regs
        size_t func_count;      //Approximation for needed functor cells
        //For non Head, non first atoms the size may be 0
        size_t y_regs_counts; //==needed y_regs

        reg_func_counts() =default;
        reg_func_counts(size_t regs, size_t funcs, size_t y_regs): x_a_regs_counts{regs}, func_count{funcs}, y_regs_counts{y_regs}{}
        reg_func_counts(size_t regs, size_t funcs ): x_a_regs_counts{regs}, func_count{funcs}, y_regs_counts{0}{}
    };
}

#endif //PROLOG_BFS_REG_FUNC_COUNTS_H
