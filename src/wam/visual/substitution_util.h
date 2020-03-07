//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_UTIL_H
#define PROLOG_BFS_SUBSTITUTION_UTIL_H

#include "../executor/executor.h"
#include "../data/functor_view.h"
#include <string>

namespace wam {
    std::string
    string_representation_of(const executor &executor, size_t index, const std::vector<functor_view> &functors,
            bool is_contigous_list = false);

    std::vector<var_heap_substitution> point_var_reg_substs_to_heap(const executor *executor);

};
#endif //PROLOG_BFS_SUBSTITUTION_UTIL_H
