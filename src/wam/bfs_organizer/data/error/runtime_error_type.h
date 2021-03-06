//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_RUNTIME_ERROR_TYPE_H
#define PROLOG_BFS_RUNTIME_ERROR_TYPE_H

enum class ERROR_TYPE{
    //Error thrown if no result has been found withing a given time_frame
    OUT_OF_TIME,
    NONE,
    OUT_OF_MEMORY,
    DIV_BY_0,
    ARGUMENTS_NOT_SUFF_INSTANCIATED,

    FUNCTOR_IS_NOT_ARITHMETIC,
};
#endif //PROLOG_BFS_RUNTIME_ERROR_TYPE_H
