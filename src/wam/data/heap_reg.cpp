//
// Created by leonhard on 09.05.20.
//


#include "heap_reg.h"

std::string wam::heap_reg::type_as_str() {
    return std::string(((const char *[]) {
            "STR",
            "REF",
            "FUN",
            "INT",
            "EXPRESSION"
    })[static_cast<size_t>(type)]);
}
