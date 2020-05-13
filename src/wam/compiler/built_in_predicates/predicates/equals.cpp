//
// Created by leonhard on 27.04.20.
//

#include "equals.h"
#include <wam/instructions/util/instructions_util.h>
#include <wam/compiler/built_in_predicates/predicates/arithmetic/util/arith_functor.h>

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

bool wam::preds::functor_equals(const executor& exec, const functor_view &fn_view, size_t lhs, size_t rhs) {
    for (int i = 0; i < fn_view.arity; ++i) {
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
    if(lhs.type != rhs.type){
        return false;
    }
    switch (lhs.type){
        case heap_tag::REF:
            return lhs_heap_i == rhs_heap_i;
        case heap_tag::CONS:
            return lhs.get_cons_i() == rhs.get_cons_i();
        case heap_tag::INT:
            return lhs.get_int_val() == rhs.get_int_val();
        case heap_tag::FUN:{
            if(lhs.get_fun_i() != rhs.get_fun_i()) return false;
            const functor_view &functor = executor.functor_of(Storage_FUN_index {lhs.index});
            return functor_equals(executor, functor, lhs_heap_i, rhs_heap_i);
        }
        case heap_tag::EVAL_FUN: {
            if (lhs.get_eval_fun_i() == rhs.get_eval_fun_i()) return false;
            const functor_view &functor = wam::arithmetic::functor_of(lhs.get_eval_fun_i());
            return functor_equals(executor, functor, lhs_heap_i, rhs_heap_i);
        }
        case heap_tag::STR:
        default:
            assert(false);
    }
}

void wam::preds::equals_check(wam::executor &executor, size_t lhs_x_reg_i, size_t rhs_x_reg_i) {
    const size_t lhs_heap_i = executor.registers[lhs_x_reg_i].get_heap_i();
    const size_t rhs_heap_i = executor.registers[rhs_x_reg_i].get_heap_i();
    if(!wam::preds::heap_reg_equals(executor, lhs_heap_i, rhs_heap_i)){
        executor.set_failed();
    }
}
