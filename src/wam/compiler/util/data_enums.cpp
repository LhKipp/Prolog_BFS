//
// Created by leonhard on 08.05.20.
//

#include "data_enums.h"
std::ostream& operator <<(std::ostream& stream,STORED_OBJECT_FLAG matrix){
    stream << "outputing STORED_OBJECT_FLAG DEBUG ONLY" << std::endl;
    return stream;
}

std::string stored_object_flag_to_str(STORED_OBJECT_FLAG flag) {
    return std::string(((const char *[]) {
            //Prolog data types
            "VARIABLE",
            "CONSTANT",
            "FUNCTOR",
            "INT",
            "SUM",
            "PRODUCT",
            "POWER",
            "PLUS",
            "MINUS",
            "INT_DIV",
            "MULT",
            "INT_POW",
            "BINARY_ARITHMETIC_PREDS_BEGIN",
            "IS",
            "BINARY_ARITHMETIC_PREDS_END",
            "NONE"
    })[static_cast<size_t>(flag)]);
}
