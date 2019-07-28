//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_TERM_H
#define PROLOG_BFS_TERM_H

#include <variant>
#include "data_types/constant.h"
#include "data_types/functor.h"
#include "data_types/variable.h"
#include "data_types/data_enums.h"

namespace prolog {


    struct term {
        std::string name;
        std::variant<constant, variable, functor> value;
    };
}



#endif //PROLOG_BFS_TERM_H
