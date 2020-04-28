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
    }
}
