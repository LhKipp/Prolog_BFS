//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_TERM_H
#define PROLOG_BFS_TERM_H

#include <memory>
#include "data_types/atom.h"
#include "data_types/functor.h"
#include "data_types/variable.h"

namespace prolog{
    const char atom_flag ='a';
    const char number_flag ='n';
    const char variable_flag ='v';
    const char functor_flag ='f';

    struct term {
//        union{
//            prolog::atom atom;
//            prolog::atom number;
//            prolog::variable var;
//            prolog::functor functor;
//        };
//        char stored_object;
//
//        term(){}
//        term(const prolog::atom& atom){
//            this->atom = atom;
//            stored_object = atom_flag;
//        }
////        term(const prolog::number& number){
////            this->number = number;
////            stored_object = number_flag;
////        }
//        term(const prolog::variable& var){
//            this->var = var;
//            stored_object = variable_flag;
//        }
//        term(const prolog::functor& func){
//            this->functor = func;
//            stored_object = functor_flag;
//        }

    };

}



#endif //PROLOG_BFS_TERM_H
