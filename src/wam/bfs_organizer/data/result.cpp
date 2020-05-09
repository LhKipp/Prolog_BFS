//
// Created by leonhard on 09.05.20.
//
#include "result.h"
#include <sstream>
#include <util/vector_util.h>

const std::string wam::result::get_answer_as_str() const {
    auto answer = get_answer();

    if(!answer){
        return "false";
    }

    //If it was an easy unification without variables
    if(answer->empty()){
        return "true";
    }

    std::stringstream result;
    result << *answer;

    return result.str();
}
