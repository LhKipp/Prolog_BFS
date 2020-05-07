//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_DATA_ENUMS_H
#define PROLOG_BFS_DATA_ENUMS_H

enum class STORED_OBJECT_FLAG{
    //Prolog data types
    VARIABLE,
    CONSTANT,
    FUNCTOR,
    INT,

    //Expressions
    EXPR,
    SUM,
    PRODUCT,
    POWER,
    //Math operators
    PLUS,
    MINUS,
    INT_DIV,
    MULT,
    INT_POW,

    NONE
};
#endif //PROLOG_BFS_DATA_ENUMS_H
