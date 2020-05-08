//
// Created by leonhard on 08.05.20.
//

#include <cmath>
#include <wam/instructions/util/instructions_util.h>
#include "expr_evaluator.h"

int wam::arithmetic::eval_int_expr(const executor &exec, const node &expr) {
    //Top level is always sum
    assert(expr.type == STORED_OBJECT_FLAG::SUM);
    return eval_int_sum(exec, expr);
}

int arithmetic::eval_int_sum(const executor &exec, const node &sum) {
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

int arithmetic::eval_int_product(const executor &exec, const node &prod){
    assert(prod.children->size() > 0);
    int total = eval_int_power(exec, (*prod.children)[0]);
    for(auto math_sign = ++prod.children->begin(); math_sign != prod.children->end(); ++math_sign){
        //Asert that after math sign value comes
        assert(math_sign + 1 != prod.children->end());
        auto power = math_sign + 1;
        if(math_sign->type == STORED_OBJECT_FLAG::MULT){
            total *= eval_int_power(exec, *power);
        }else{// if its minus
            assert(math_sign->type == STORED_OBJECT_FLAG::INT_DIV);
            total /= eval_int_power(exec, *power);
        }

        ++math_sign;
    }

    return total;
}

int arithmetic::eval_int_power(const executor &exec, const node &power){
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

int arithmetic::eval_int_value(const executor &exec, const node &value){
    if(value.is_expr()){
        return eval_int_expr(exec, value);
    }else if(value.is_int()){
        return std::stoi(value.name);
    }else{
        assert(value.is_variable());

        size_t heap_addr;
        if(value.is_permanent()){
            heap_addr = wam::deref(exec, exec.registers.at(value.get_a_reg()));
        }else{
            heap_addr = wam::deref(exec, exec.registers.at(value.get_x_reg()));
        }
        auto reg = exec.heap_at(heap_addr);

        if(!reg.is_INT()){
            //Throw runtime error
            assert(false);
        }

        return reg.get_int_val();
    }
}
