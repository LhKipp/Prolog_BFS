//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_TERM_H
#define PROLOG_BFS_TERM_H

#include <memory>
#include <cstring>
#include "data_types/constant.h"
#include "data_types/functor.h"
#include "data_types/variable.h"

namespace prolog{

    enum STORED_OBJECT_FLAG{
        VARIABLE,
        CONSTANT,
        FUNCTOR,
        NONE
    };

    struct term {
        std::string name;
        std::vector<term> terms;
        STORED_OBJECT_FLAG stored_obj_flag;

        term()=default;
        term(const std::string name, STORED_OBJECT_FLAG flag): name{name}, stored_obj_flag{flag}, terms{0}{}

        bool is_const(){
            return stored_obj_flag == CONSTANT;
        }
        bool is_functor(){
            return stored_obj_flag == FUNCTOR;
        }
        bool is_variable(){
            return stored_obj_flag = VARIABLE;
        }
    };

}



#endif //PROLOG_BFS_TERM_H
