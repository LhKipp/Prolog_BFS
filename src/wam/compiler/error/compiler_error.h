//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_COMPILER_ERROR_H
#define PROLOG_BFS_COMPILER_ERROR_H

#include <wam/data/source_code_info.h>
#include "error_type.h"

namespace compiler{
    struct error{
        ERROR_TYPE type;
        wam::source_code_info atom;
        std::string description;

        error(ERROR_TYPE type, const wam::source_code_info &atom, const std::string &description) : type(type),
                                                                                                    atom(atom),
                                                                                                    description(
                                                                                                            description) {}
    };
}

#endif //PROLOG_BFS_COMPILER_ERROR_H
