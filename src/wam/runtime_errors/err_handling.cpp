//
// Created by leonhard on 09.05.20.
//

#include "err_handling.h"
#include <wam/instructions/util/instructions_util.h>

void err_handling::check_and_throw_is_evaluable(wam::executor &exec, wam::heap_reg reg) {
    if(reg.is_FUN() || reg.is_CONS()){
        auto fn_view = exec.functor_of(FUN_index{reg.index});
        exec.set_runtime_error(runtime_error{
                ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC,
                exec.get_current_term_code()->get_code_info(),
                "ERROR: Arithmetic: " + fn_view.to_string() + " is not a function"
        });
        throw ERROR_TYPE ::FUNCTOR_IS_NOT_ARITHMETIC;
    }else if(reg.is_REF()){
        auto var = exec.var_name_of(Storage_Var_index{reg.var_index});
        exec.set_runtime_error(runtime_error{
                ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED,
                exec.get_current_term_code()->get_code_info(),
                "ERROR: Arguments are not sufficiently instantiated"
                "Hint: " + var + " points to nothing and is therefore not evaluable"
        });
            throw ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED;
    }
}
