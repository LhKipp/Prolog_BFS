//
// Created by leonhard on 16.05.20.
//

#include "prolog_bfs/wam/compiler/error/compiler_error.h"
#include "prolog_bfs/wam/compiler/checks/redefinition_of_built_in_pred.h"

void compiler::check_and_throw_redefinition_of_built_in_pred(const node& head_func) {
    if((head_func.name == "append" && head_func.get_arity() == 3)
       || (head_func.name == "is" && head_func.get_arity() == 2)){
        throw compiler::error{
            ERROR_TYPE ::REDEFINITION_OF_BUILT_IN_PREDICATE,
            head_func.code_info,
            "Redefinition of built in predicate " + head_func.name + " is prohibited.",
            0
        };
    }
}
