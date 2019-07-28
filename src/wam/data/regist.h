//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_REGIST_H
#define PROLOG_BFS_REGIST_H


#include <cstddef>
#include "heap_tag.h"

namespace wam {
    struct regist {
        heap_tag type;
        size_t index;

        regist()= default;
        regist(heap_tag type, size_t index): type{type}, index{index}{}
    };
}


#endif //PROLOG_BFS_REGIST_H
