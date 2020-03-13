//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_HEAP_TAG_H
#define PROLOG_BFS_HEAP_TAG_H

#include "../compiler/util/node.h"

namespace wam {
    enum class heap_tag {
        STR,
        REF,
        FUN
    };
}

#endif //PROLOG_BFS_HEAP_TAG_H
