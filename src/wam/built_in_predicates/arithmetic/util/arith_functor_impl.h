//
// Created by leonhard on 11.05.20.
//

#ifndef PROLOG_BFS_ARITH_FUNCTOR_IMPL_H
#define PROLOG_BFS_ARITH_FUNCTOR_IMPL_H

#include <cmath>

namespace wam::arithmetic{
    float pow(float base, float exp){
        return std::pow(base, exp);
    }
}

#endif //PROLOG_BFS_ARITH_FUNCTOR_IMPL_H
