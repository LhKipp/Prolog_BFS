//
// Created by leonhard on 29.07.19.
//
#include "substitution_util.h"
#include "../../instructions/util/instructions_util.h"
#include "../../config/config.h"
#include "../../data/regist.h"



//TODO recursive function, could be optimized through use of stack
std::string
wam::string_representation_of(const std::vector<wam::regist> &registers, size_t index,
                              const std::vector<functor_view> &functors,
                              //TODO Better Naming: is_contigous_list should be is_list_start
                              //Need to negate part of the logic
                              bool is_contigous_list) {
    //If register is an Ref cell we try to dereference it
    if (registers[index].is_REF()) {
        index = wam::deref(registers, registers[index]);
    }

    //If the register is still a ref it is an unbound ref cell
    if (registers[index].is_REF()) {
        return std::string{wam::config::UNBOUND_VAR_PREFIX} + std::to_string(index);
    }

    if (registers[index].is_STR()) {
        index = registers[index].index;
    }

    //At this point the register will be an FUN cell
    const functor_view &functor = functors[registers[index].index];

    //If the functor is the empty list, we return nothing
    if(functor.is_empty_list()){
        if(!is_contigous_list){
            return "[]";
        }else{
            return "]";
        }
    }

    //If the functor is a constant, we return the name
    if (functor.is_constant()) {
        return functor.name;
    }

    //Either list or normal functor
    std::string result;

    //If the functor is a list
    if (functor.is_list()){
        if(!is_contigous_list){
            result = "[";
        }
        result += string_representation_of(registers, index +1, functors, false) + ",";
        result += string_representation_of(registers, index +2, functors, true);

        //If the list has been completly built, remove the , before the ]
        if(!is_contigous_list){
            result.erase(result.cend() -2);
        }
        return result;
    }

    //The functor is a normal functor
    result += functor.name + '(';
    //-1 for correct formatting of the ,
    for (int i = 1; i <= functor.arity - 1; ++i) {
        result += string_representation_of(registers, index + i, functors) + ',';
    }
    return result + string_representation_of(registers, index + functor.arity, functors) + ")";
}


