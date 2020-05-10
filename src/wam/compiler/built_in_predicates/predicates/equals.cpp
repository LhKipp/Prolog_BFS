//
// Created by leonhard on 27.04.20.
//

#include "equals.h"
#include <wam/instructions/util/instructions_util.h>

node wam::preds::equals_node_tree() {
    node pred{STORED_OBJECT_FLAG::FUNCTOR, "=="};
    node lhs{STORED_OBJECT_FLAG ::VARIABLE, "Lhs"};
    node rhs{STORED_OBJECT_FLAG ::VARIABLE, "Rhs"};

    pred.code_info.line_begin = 0;
    pred.code_info.line_end = 0;
    pred.code_info.value = "Lhs == Rhs";
    pred.add_to_children(lhs);
    pred.add_to_children(rhs);

    return pred;
}

/*
 * This method assumes that if lhs_heap_i or rhs_heap_i is ref, it cant be further dereferenced
 */
bool wam::preds::heap_reg_equals(wam::executor& executor, size_t lhs_heap_i, size_t rhs_heap_i){
    heap_reg lhs = executor.heap_at(lhs_heap_i);
    heap_reg rhs = executor.heap_at(rhs_heap_i);
    if(lhs.is_REF()){
        lhs_heap_i = wam::deref(executor, lhs.index);
        lhs = executor.heap_at(lhs_heap_i);
    }
    if(rhs.is_REF()){
        rhs_heap_i = wam::deref(executor, rhs.index);
        rhs = executor.heap_at(rhs_heap_i);
    }
    if(lhs.is_STR()){
        lhs_heap_i = lhs.index;
        lhs = executor.heap_at(lhs_heap_i);
    }
    if(rhs.is_STR()){
        rhs_heap_i = rhs.index;
        rhs = executor.heap_at(rhs_heap_i);
    }

    if(lhs.type != rhs.type){
        return false;
    }
    if(lhs.is_REF()){
        assert(rhs.is_REF());
        //Always false because of check at top
        //return lhs_heap_i == rhs_heap_i;
        return lhs_heap_i == rhs_heap_i;
    }
    if(lhs.is_INT()){
        assert(rhs.is_INT());
        return lhs.get_int_val() == rhs.get_int_val();
    }
    if(lhs.is_FUN()){
        assert(rhs.is_FUN());
        const functor_view &functor1 = executor.functor_of(Storage_FUN_index {lhs.index});
        const functor_view &functor2 = executor.functor_of(Storage_FUN_index {rhs.index});
        if(functor1 != functor2){
            return false;
        }

        for (int i = 0; i < functor1.arity; ++i) {
            if(!heap_reg_equals(executor,
                    lhs_heap_i + 1 + i,
                    rhs_heap_i + 1 + i)) {
                return false;
            }
        }
        return true;
    }
    //Didnt expect the type of lhs
    assert(false);
}

void wam::preds::equals_check(wam::executor &executor, size_t lhs_x_reg_i, size_t rhs_x_reg_i) {
    const size_t lhs_heap_i = executor.registers[lhs_x_reg_i].heap_i;
    const size_t rhs_heap_i = executor.registers[rhs_x_reg_i].heap_i;
    if(!wam::preds::heap_reg_equals(executor, lhs_heap_i, rhs_heap_i)){
        executor.set_failed();
    }
}
