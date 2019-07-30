//
// Created by leonhard on 29.07.19.
//
#include "substitution_util.h"
#include "../../instructions/util/instructions_util.h"
#include "../../config/config.h"


std::string string_representation_of(const wam::executor& executor, size_t index){
    const std::vector<wam::regist>& registers = executor.heap;


    //If register is an Ref cell we try to dereference it
    if(registers[index].is_REF()){
        index = wam::deref(registers, registers[index]);
    }

    //If the register is still a ref it is an unbound ref cell
    if(registers[index].is_REF()){
        return std::string{wam::config::UNBOUND_VAR_PREFIX} + std::to_string(index);
    }

    if(registers[index].is_STR()){
        index = registers[index].index;
    }

    //At this point the register will be an FUN cell
    const functor_view& functor = executor.functor_of(wam::FUN_index{index});
    std::string result = functor.name + '(';
    //-1 for correct formatting of the ,
    for(int i= 1; i <= functor.arity -1; ++i){
           result += string_representation_of(executor, index + i);
    }
    return result + string_representation_of(executor,index + functor.arity) + ')';

}
wam::var_substitution
wam::substitution_for(const wam::var_reg_substitution &var_reg_sub, const wam::executor &executor) {
    wam::var_substitution result;
    result.var_name = var_reg_sub.var_name;

    result.substitute = string_representation_of(executor, var_reg_sub.register_index);

    regist reg = executor.heap[var_reg_sub.register_index];

    //If reg is Ref we try to dereference it
    if(reg.is_REF()){
        reg = executor.heap[wam::deref(executor.heap, reg)];
    }

    //If reg is still Ref it is a unbound Variable
    if(reg.is_REF()){
        result.substitute = result.var_name;
    }else if (reg.is_FUN()){
        //If reg is Fun we need to copy the whole! functor

    }
}

wam::var_substitutions
wam::substitutions_for(const wam::var_reg_substitutions &substitutions, const wam::executor &executor) {
    wam::var_substitutions result{substitutions.size()};

    std::transform(substitutions.begin(), substitutions.end(), result.begin(),
                   [&](const var_reg_substitution &var_reg_sub){
                        return substitution_for(var_reg_sub, executor);
                   });

    return result;
}
