//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_DATA_ENUMS_H
#define PROLOG_BFS_DATA_ENUMS_H

#include <ostream>
enum class STORED_OBJECT_FLAG{
    //Prolog data types
    VARIABLE,
    CONSTANT,
    FUNCTOR,
    INT,

    //Expressions
    SUM,
    PRODUCT,
    POWER,
    //Math operators
    PLUS,
    MINUS,
    INT_DIV,
    MULT,
    INT_POW,

    //Arithmetic binary operators usable as query atoms
    BINARY_ARITHMETIC_PREDS_BEGIN,
    IS,
    BINARY_ARITHMETIC_PREDS_END,


    NONE
};

std::ostream& operator <<(std::ostream& stream, STORED_OBJECT_FLAG matrix);
#endif //PROLOG_BFS_DATA_ENUMS_H
