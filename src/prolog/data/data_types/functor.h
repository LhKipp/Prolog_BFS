//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_FUNCTOR_H
#define PROLOG_BFS_FUNCTOR_H


#include <string>
#include <vector>

namespace prolog {

    struct term;
    struct functor {
        std::string name;
        std::vector<prolog::term> args;

        functor(){}
        functor(const std::string name, const std::vector<prolog::term>& args) {
            this->name = name;
            this->args = args;
        }
    };

}


#endif //PROLOG_BFS_FUNCTOR_H
