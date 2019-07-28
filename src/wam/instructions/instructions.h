//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_INSTRUCTIONS_H
#define PROLOG_BFS_INSTRUCTIONS_H

#include "../executor/executor.h"
#include "../parser/util/node.h"

namespace wam {
    void put_structure(executor& executor, const regist& functor, regist& x_reg){
        executor.heap.emplace_back(heap_tag::STR, executor.heap.size() + 1);
        x_reg = executor.heap.back();
        executor.heap.push_back(functor);
    }

    void set_variable(executor& executor , regist& x_reg){
        executor.heap.emplace_back(heap_tag::REF, executor.heap.size());
        x_reg = executor.heap.back();
    }

    void set_value(executor& executor, regist& x_reg){
        executor.heap.push_back(x_reg);
    }
}

#endif //PROLOG_BFS_INSTRUCTIONS_H
