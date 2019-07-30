//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_INSTRUCTIONS_H
#define PROLOG_BFS_INSTRUCTIONS_H

#include "../executor/executor.h"

namespace wam {
    void put_structure(executor& executor, const functor_view& functor, size_t x_reg);

    void set_variable(executor& executor , size_t x_reg);

    void set_value(executor& executor,size_t x_reg);

    void get_structure(executor& executor,const functor_view& functor, size_t x_reg);

    void unify_variable(executor& executor,  size_t x_reg);

    void unify_value(executor& executor, size_t x_reg);


    void bind(std::vector<regist>& store, size_t address_a,size_t address_b);

    void unify(executor& executor, size_t addr_a, size_t addr_b);
}

#endif //PROLOG_BFS_INSTRUCTIONS_H
