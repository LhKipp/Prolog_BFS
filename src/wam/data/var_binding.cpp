//
// Created by leonhard on 31.07.19.
//
#include "var_binding.h"

std::ostream &wam::operator<<(std::ostream &os, const wam::var_binding &substitution) {
    os << "[" << substitution.var_name << " / " << substitution.binding << "]";
    return os;
}
