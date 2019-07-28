//
// Created by leonhard on 24.07.19.
//

#ifndef PROLOG_BFS_EXECUTOR_H
#define PROLOG_BFS_EXECUTOR_H


#include <vector>
#include "../data/regist.h"
#include "../data/functor_view.h"

namespace wam {
    struct executor {

        std::vector<regist> registers;
        std::vector<functor_view> functors;

        std::vector<regist> heap;
    };
}


#endif //PROLOG_BFS_EXECUTOR_H
