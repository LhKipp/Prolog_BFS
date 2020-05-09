//
// Created by leonhard on 09.05.20.
//

#include "err_handling.h"
#include <wam/instructions/util/instructions_util.h>

void err_handling::check_and_throw_is_evaluable(wam::executor &exec, const wam::regist &reg) {
    if(reg.is_STR()){
        auto fn_view = exec.functor_of(STR_index{reg.index});
        exec.set_runtime_error(runtime_error{
                ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC,
                exec.get_current_term_code()->get_code_info(),
                "ERROR: Arithmetic: " + fn_view.to_string() + " is not a function"
        });
        throw ERROR_TYPE ::FUNCTOR_IS_NOT_ARITHMETIC;
    }else if(reg.is_FUN()){
        //Assert rhs is arithmetic func (sin, cos ...) for now we simply throw a runtime error
        auto fn_view = exec.functor_of(FUN_index{reg.index});
        exec.set_runtime_error(runtime_error{
                ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC,
                exec.get_current_term_code()->get_code_info(),
                "ERROR: Arithmetic: " + fn_view.to_string() + " is not a function"
        });
        throw ERROR_TYPE ::FUNCTOR_IS_NOT_ARITHMETIC;
    }else if(reg.is_REF()){
        auto derefed_reg_i = wam::deref(exec, reg.index);
        auto derefed_reg = exec.heap_at(derefed_reg_i);
        //if reg is unbound
        if(derefed_reg.index == derefed_reg_i){
            auto var = exec.var_name_of(Storage_Var_index{derefed_reg.var_index});
            exec.set_runtime_error(runtime_error{
                    ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED,
                    exec.get_current_term_code()->get_code_info(),
                    "ERROR: Arguments are not sufficiently instantiated"
                    "Hint: " + var + " points to nothing and is therefore not evaluable"
            });
            throw ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED;
        }else{
            check_and_throw_is_evaluable(exec, derefed_reg);
        }
    }
}
