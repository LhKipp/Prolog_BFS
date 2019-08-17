//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_UTIL_H
#define PROLOG_BFS_SUBSTITUTION_UTIL_H

#include "../../executor/executor.h"

namespace wam {
    std::string
    string_representation_of(const std::vector<wam::regist> &store, size_t index, const std::vector<functor_view> &functors,
            bool is_contigous_list = false);

};
#endif //PROLOG_BFS_SUBSTITUTION_UTIL_H
