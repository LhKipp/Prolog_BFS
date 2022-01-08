//
// Created by leonhard on 08.05.20.
//

#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/expr_evaluator.h"
#include "prolog_bfs/wam/instructions/util/instructions_util.h"
#include "prolog_bfs/wam/built_in_predicates/arithmetic/is.h"

void wam::preds::is(wam::executor &exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i){
    const heap_reg& rhs_value = wam::arithmetic::eval_arithmetic_reg(exec,
                                                                     exec.registers[rhs_x_reg_i].get_heap_i());
    heap_reg& lhs = wam::derefed_reg_modify(exec, exec.registers.at(lhs_x_reg_i).get_heap_i());
    //If people use is like =:=
    switch(lhs.type){
        case heap_tag::INT:
            if(lhs.get_int_val() != rhs_value.get_int_val()){
                exec.set_failed();
            }
            return;
        case heap_tag::REF:
            exec.push_back(rhs_value);
            lhs.bind_to((int)exec.heap_size() - 1);
            return;
        case heap_tag::CONS:
        case heap_tag::FUN:
        case heap_tag::EVAL_FUN:
        case heap_tag::STR:
            exec.set_failed();
            return;
    }
}

