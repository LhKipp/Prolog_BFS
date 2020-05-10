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
    //This is very similar to the unify algorithm
#ifdef DEBUG
    std::cout << "equals_check" << std::endl;
#endif
    std::stack<size_t> PDL;
    PDL.push(executor.registers[lhs_x_reg_i].index);
    PDL.push(executor.registers[rhs_x_reg_i].index);

    while (!(PDL.empty() || executor.failed())) {
        size_t d1, d2;
        if (executor.heap_at(PDL.top()).is_REF()){
            d1 = deref(executor, executor.heap_at(PDL.top()));
        } else {
            d1 = PDL.top();
        }
        PDL.pop();
        if (executor.heap_at(PDL.top()).is_REF()){
            d2 = deref(executor, executor.heap_at(PDL.top()));
        } else {
            d2 = PDL.top();
        }
        PDL.pop();

        if (executor.heap_at(d1).is_FUN()) {
            --d1;
        }
        if (executor.heap_at(d2).is_FUN()) {
            --d2;
        }

        if (d1 != d2) {
            const regist &reg1 = executor.heap_at(d1);
            const regist &reg2 = executor.heap_at(d2);

            //If different registers and one of them is ref, its always fail
            if (reg1.is_REF() || reg2.is_REF()) {
                executor.set_failed();
                return;
            } else {
                //If both registers are str (functors) we check that all inner terms are equal
                //reg1 && reg2 are STR registers
                const functor_view &functor1 = executor.functor_of(FUN_index{reg1.index});
                const functor_view &functor2 = executor.functor_of(FUN_index{reg2.index});

                if (functor1 == functor2) {
                    for (int i = 1; i <= functor1.arity; ++i) {
                        PDL.push(reg1.index + i);
                        PDL.push(reg2.index + i);
                    }
                } else {
                    executor.set_failed();
                    return;
                }
            }
        }
    //Var equality means same var_string_name and same register in heap
    //But both vars having same x_reg also means they are identical
    const size_t lhs_heap_i = executor.registers[lhs_x_reg_i].heap_i;
    const size_t rhs_heap_i = executor.registers[rhs_x_reg_i].heap_i;
    if(!wam::preds::heap_reg_equals(executor, lhs_heap_i, rhs_heap_i)){
        executor.set_failed();
    }
}
