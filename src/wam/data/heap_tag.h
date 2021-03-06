//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_HEAP_TAG_H
#define PROLOG_BFS_HEAP_TAG_H

#include <cstdint>

namespace wam {
    enum class heap_tag : uint8_t{
        CONS,
        INT,
        REF,
        FUN,
        EVAL_FUN,
        STR,
    };
}

#endif //PROLOG_BFS_HEAP_TAG_H
