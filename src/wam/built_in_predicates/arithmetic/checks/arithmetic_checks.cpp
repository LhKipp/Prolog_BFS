//
// Created by leonhard on 11.05.20.
//

#include "arithmetic_checks.h"
#include <wam/built_in_predicates/arithmetic/util/arith_functor.h>

void wam::arithmetic::check_and_throw_if_wrong_values(executor &executor, const heap_reg &func, const heap_reg& lhs, const heap_reg& rhs) {
    if((func.index == get_int_div_i() || func.index == get_div_i())
        && rhs.get_int_val() == 0){
        executor.set_runtime_error(runtime_error{
                ERROR_TYPE::DIV_BY_0,
                executor.get_current_term_code()->get_code_info(),
                "ERROR: Arithmetic: evaluation error: zero_divisor"
        });
        throw ERROR_TYPE ::DIV_BY_0;
    }
}
