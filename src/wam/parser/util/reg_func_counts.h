//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_REG_FUNC_COUNTS_H
#define PROLOG_BFS_REG_FUNC_COUNTS_H

#include <cstddef>

namespace wam::helper {
    struct reg_func_counts {
        size_t reg_count;
        size_t func_count;

        reg_func_counts() =default;
        reg_func_counts(size_t regs, size_t funcs): reg_count{regs}, func_count{funcs}{}
    };
}

#endif //PROLOG_BFS_REG_FUNC_COUNTS_H
