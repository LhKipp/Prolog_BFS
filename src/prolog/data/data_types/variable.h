//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_VARIABLE_H
#define PROLOG_BFS_VARIABLE_H

#include <string>

namespace prolog {
    struct variable {
        std::string name;
        variable(){
         name = "";
        }
        variable(const std::string name){
            this->name= name;
        }
    };

}

#endif //PROLOG_BFS_VARIABLE_H
