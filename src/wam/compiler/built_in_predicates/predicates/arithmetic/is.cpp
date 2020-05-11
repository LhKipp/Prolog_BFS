//
// Created by leonhard on 08.05.20.
//

#include <wam/compiler/built_in_predicates/predicates/arithmetic/util/expr_evaluator.h>
#include <wam/instructions/util/instructions_util.h>
#include "is.h"

node wam::preds::is_node_tree() {
    node pred{STORED_OBJECT_FLAG::FUNCTOR, "is"};
    node lhs{STORED_OBJECT_FLAG ::VARIABLE, "Lhs"};
    node rhs{STORED_OBJECT_FLAG ::VARIABLE, "Rhs_expr"};

    pred.code_info.line_begin = 0;
    pred.code_info.line_end = 0;
    pred.code_info.value = "Lhs is Expr";
    pred.add_to_children(lhs);
    pred.add_to_children(rhs);

    return pred;
}

void wam::preds::is(wam::executor &exec, size_t lhs_x_reg_i, size_t rhs_x_reg_i){
    const heap_reg& rhs_value = wam::arithmetic::eval_arithmetic_reg(exec,
                                                                     exec.registers[rhs_x_reg_i].heap_i);
    heap_reg& lhs = exec.registers.at(lhs_x_reg_i).reg;
    //If people use is like =:=
    if(lhs.is_INT() ) {
        if(lhs.get_int_val() != rhs_value.get_int_val()){
            exec.set_failed();
        }
        return;
    }else if(lhs.is_FUN() || lhs.is_STR() || lhs.is_EVAL_FUN()) {
        exec.set_failed();
        return;
    }else if(lhs.is_REF()){
        heap_reg& derefed_reg = wam::derefed_reg_modify(exec, lhs);
        //if var already assigned before
        if(derefed_reg.is_INT()){
            if(derefed_reg.get_int_val() != rhs_value.get_int_val()){
                exec.set_failed();
            }
            return;
        }else if(derefed_reg.is_REF()){
            //TODO check whether you can just push some ints on the stack
            //I think its fine
            exec.push_back(rhs_value);
            derefed_reg.bind_to((int)exec.heap_size() - 1);
            return;
        }else{
            //its list fun const or something. Yes this is the actual behavior, no runtime exc for that
            exec.set_failed();
//            //derefed_reg is something else than ref or int
//            exec.set_runtime_error(runtime_error{
//                    ERROR_TYPE ::ARGUMENTS_NOT_SUFF_INSTANCIATED,
//                    exec.get_current_term_code()->get_code_info(),
//                    "In is/2 predicate: \
//                    Lhs is a Variable pointing to a " + lhs.type_as_str() +
//                    "\nExpected Lhs to be of type INT or REF"
//            });
            return;
        }
    }
}

