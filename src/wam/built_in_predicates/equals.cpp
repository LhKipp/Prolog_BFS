//
// Created by leonhard on 27.04.20.
//

#include "prolog_bfs/wam/built_in_predicates/equals.h"

#include <prolog_bfs/wam/instructions/util/instructions_util.h>

bool wam::preds::functor_equals(const executor& exec, int arity, size_t lhs, size_t rhs) {
    for (int i = 0; i < arity; ++i) {
        if(!heap_reg_equals(exec,
                            lhs + 1 + i,
                            rhs + 1 + i)) {
            return false;
        }
    }
    return true;
}

bool wam::preds::heap_reg_equals(const wam::executor& executor, size_t lhs_heap_i, size_t rhs_heap_i){
    lhs_heap_i = wam::deref(executor, lhs_heap_i);
    rhs_heap_i = wam::deref(executor, rhs_heap_i);
    const heap_reg lhs = executor.heap_at(lhs_heap_i);
    const heap_reg rhs = executor.heap_at(rhs_heap_i);
    //shallow check
    if(lhs != rhs){
        return false;
    }
    //for refs assert heap_i equals also
    if(lhs.is_REF()){
        return lhs_heap_i == rhs_heap_i;
    }
    //For functors we also check the children
    if(lhs.is_EVAL_FUN() || lhs.is_FUN()){
        return functor_equals(executor, lhs.get_arity(), lhs_heap_i, rhs_heap_i);
    }

    return true;
}

void wam::preds::equals_check(wam::executor &executor, size_t lhs_x_reg_i, size_t rhs_x_reg_i) {
    const size_t lhs_heap_i = executor.registers[lhs_x_reg_i].get_heap_i();
    const size_t rhs_heap_i = executor.registers[rhs_x_reg_i].get_heap_i();
    if(!wam::preds::heap_reg_equals(executor, lhs_heap_i, rhs_heap_i)){
        executor.set_failed();
    }
}
