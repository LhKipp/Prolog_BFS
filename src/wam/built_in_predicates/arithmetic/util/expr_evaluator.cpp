//
// Created by leonhard on 08.05.20.
//

#include "expr_evaluator.h"
#include "arith_functor.h"
#include <cmath>
#include <wam/instructions/util/instructions_util.h>
#include <wam/runtime_errors/err_handling.h>
#include <wam/built_in_predicates/arithmetic/checks/arithmetic_checks.h>

wam::heap_reg wam::arithmetic::eval_arithmetic_reg(executor &exec, size_t rhs_heap_i){
    rhs_heap_i = deref(exec, rhs_heap_i);
    heap_reg rhs = exec.heap_at(rhs_heap_i);
    err_handling::check_and_throw_is_evaluable(exec, rhs);

    switch (rhs.type){
        case heap_tag::INT:
            return rhs;
        case heap_tag::REF:
            return eval_arithmetic_reg(exec, wam::deref(exec, rhs));
        case heap_tag::EVAL_FUN :{
            return eval_arithmetic_func(exec, rhs, rhs_heap_i);
        }
            //Not reachable
        case heap_tag::CONS:
        case heap_tag::STR:
        default: assert(false);
    }
    //Not reachable
    assert(false);
}

wam::heap_reg arithmetic::eval_arithmetic_func(executor &exec, heap_reg &func, size_t func_heap_i) {
    //for now only int arithmetic is implemented
    const int arity = arity_of(func);
    double value;
    if(arity == 1){
        auto& function = get_unary_func_of(func);
        const heap_reg arg = eval_arithmetic_reg(exec, func_heap_i + 1);
        //No such check for now
//        check_and_throw_if_wrong_values(exec, func, arg);
        //for now everything is int
        value = function(arg.get_int_val());
    }else{//binary
        const heap_reg lhs = eval_arithmetic_reg(exec, func_heap_i + 1);
        const heap_reg rhs = eval_arithmetic_reg(exec, func_heap_i + 2);
        check_and_throw_if_wrong_values(exec, func, lhs, rhs);

        auto& function = get_binary_func(func);
        value = function(lhs.get_int_val(), rhs.get_int_val());
    }

    return wam::heap_reg{
            heap_tag::INT,
            (int) value
    };
}
