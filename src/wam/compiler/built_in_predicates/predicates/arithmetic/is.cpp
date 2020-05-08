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
    regist& rhs_expr = exec.registers.at(rhs_x_reg_i);
    const node& expr = exec.expr_of(Storage_Expr_index{rhs_expr.get_expr_i()});
    int rhs_value = wam::arithmetic::eval_int_expr(exec, expr);

    regist& lhs = exec.registers.at(lhs_x_reg_i);
    //If people use is like =:=
    if(lhs.is_INT() ) {
        if(lhs.get_int_val() != rhs_value){
            exec.set_failed();
        }
        return;
    }

    //lhs is a variable
    assert(lhs.is_REF());
    size_t deref_reg = wam::deref(exec, lhs.index);
    regist& reg = exec.heap_modify(deref_reg);
    //if var already assigned before
    if(reg.is_INT()){
        if(reg.get_int_val() != rhs_value){
            exec.set_failed();
        }
        return;
    }

    if(reg.is_REF()){
        //TODO check whether you can just push some ints on the stack
        exec.push_back_int(rhs_value);
        reg.bind_to(exec.heap_size() -1);
        return;
    }

    //reg is neither int nor ref --> runtime error
    //TODO runtime error
    exec.set_failed();
    assert(false);


}

