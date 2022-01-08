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
    EVALUABLE_FUNCTOR,
    NONE
};

std::string stored_object_flag_to_str(STORED_OBJECT_FLAG f);
std::ostream& operator <<(std::ostream& stream, STORED_OBJECT_FLAG f);
#endif //PROLOG_BFS_DATA_ENUMS_H
