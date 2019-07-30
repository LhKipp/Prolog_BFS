//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_INSTRUCTIONS_UTIL_H
#define PROLOG_BFS_INSTRUCTIONS_UTIL_H

#include <cstddef>
#include "../../data/regist.h"

namespace wam{
    size_t deref(const std::vector<regist>& store,const regist& init_regist);
}
#endif //PROLOG_BFS_INSTRUCTIONS_UTIL_H
