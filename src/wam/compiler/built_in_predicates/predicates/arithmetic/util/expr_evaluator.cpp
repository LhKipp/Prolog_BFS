//
// Created by leonhard on 08.05.20.
//

#include <cmath>
#include <wam/instructions/util/instructions_util.h>
#include <wam/runtime_errors/err_handling.h>
#include "expr_evaluator.h"

int wam::arithmetic::eval_int_expr(executor &exec, const regist &rhs_expr) {
    err_handling::check_and_throw_is_evaluable(exec, rhs_expr);

    if(rhs_expr.is_INT()){
        return rhs_expr.get_int_val();
    }else if(rhs_expr.is_REF()){
        auto derefed_reg = wam::derefed_reg(exec, rhs_expr);
        return eval_int_expr(exec, derefed_reg);
    }else{
        //rhs is expr
        const node& expr = exec.expr_of(Storage_Expr_index{rhs_expr.get_expr_i()});
        //Top level is always sum
        assert(expr.type == STORED_OBJECT_FLAG::SUM);
        return eval_int_sum(exec, expr);
    }
}

int arithmetic::eval_int_sum(executor &exec, const node &sum) {
    assert(sum.children->size() > 0);
    int total = eval_int_product(exec, (*sum.children)[0]);
    for(auto math_sign = ++sum.children->begin(); math_sign != sum.children->end(); ++math_sign){
        //Asert that after math sign value comes
        assert(math_sign + 1 != sum.children->end());
        auto prod = math_sign + 1;
        if(math_sign->type == STORED_OBJECT_FLAG::PLUS){
            total += eval_int_product(exec, *prod);
        }else{// if its minus
            assert(math_sign->type == STORED_OBJECT_FLAG::MINUS);
            total -= eval_int_product(exec, *prod);
        }

        ++math_sign;
    }

    return total;
}

int arithmetic::eval_int_product(executor &exec, const node &prod){
    assert(prod.children->size() > 0);
    int total = eval_int_power(exec, (*prod.children)[0]);
    for(auto math_sign = ++prod.children->begin(); math_sign != prod.children->end(); ++math_sign){
        //Asert that after math sign value comes
        assert(math_sign + 1 != prod.children->end());
        auto power = math_sign + 1;
        if(math_sign->type == STORED_OBJECT_FLAG::MULT){
            total *= eval_int_power(exec, *power);
        }else{// if its int_div
            assert(math_sign->type == STORED_OBJECT_FLAG::INT_DIV);

            //Check that we dont div by 0
            int rhs = eval_int_power(exec, *power);
            if(rhs == 0){
                exec.set_runtime_error(runtime_error{
                    ERROR_TYPE::DIV_BY_0,
                    exec.get_current_term_code()->get_code_info(),
                    "ERROR: Arithmetic: evaluation error: zero_divisor"
                });
                throw ERROR_TYPE ::DIV_BY_0;
            }
            total /= rhs;
        }

        ++math_sign;
    }

    return total;
}

int arithmetic::eval_int_power(executor &exec, const node &power){
    assert(power.children->size() > 0);
    int total = eval_int_value(exec, (*power.children)[0]);
    for(auto math_sign = ++power.children->begin(); math_sign != power.children->end(); ++math_sign){
        //Asert that after math sign value comes
        assert(math_sign + 1 != power.children->end());
        auto value = math_sign + 1;

        assert(math_sign->type == STORED_OBJECT_FLAG::INT_POW);
        total = std::pow(total, eval_int_value(exec, *value));

        ++math_sign;
    }

    return total;
}

int arithmetic::eval_int_value(executor &exec, const node &value){
    if(value.is_expr()){
        return eval_int_sum(exec, value);
    }else if(value.is_int()){
        return std::stoi(value.name);
    }else{
        assert(value.is_variable());

        regist derefed_reg;
        if(value.is_permanent()){
            derefed_reg = wam::derefed_reg(exec, exec.cur_permanent_registers().at(value.get_y_reg()));
        }else{
            derefed_reg = wam::derefed_reg(exec, exec.registers.at(value.get_x_reg()));
        }
        err_handling::check_and_throw_is_evaluable(exec, derefed_reg);
        //if var pointed to a expression
        if(derefed_reg.is_EXPR()){
            return eval_int_expr(exec, derefed_reg);
        }else{
            assert(derefed_reg.is_INT());
            return derefed_reg.get_int_val();
        }
    }
}
