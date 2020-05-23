//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_COMPILER_ERROR_TYPE_H
#define PROLOG_BFS_COMPILER_ERROR_TYPE_H

namespace compiler{
    enum class ERROR_TYPE{
        NONE,
        PARSER_ERROR,
        //Usage of uninit var
        ARGUMENTS_NOT_SUFF_INSTANCIATED,
        REDEFINITION_OF_BUILT_IN_PREDICATE,

    };
}
#endif //PROLOG_BFS_COMPILER_ERROR_TYPE_H
