//
// Created by leonhard on 09.05.20.
//


#include "prolog_bfs/wam/data/heap_reg.h"

std::string wam::heap_reg::type_as_str() {
    return std::string(((const char *[]) {
            "CONS",
            "INT",
            "REF",
            "FUN",
            "EVAL_FUN",
            "STR",
    })[static_cast<size_t>(type)]);
}

bool wam::heap_reg::operator==(const heap_reg &rhs) const {
    return index == rhs.index &&
           var_index == rhs.var_index &&
           type == rhs.type;
}

bool wam::heap_reg::operator!=(const heap_reg &rhs) const {
    return !(rhs == *this);
}
