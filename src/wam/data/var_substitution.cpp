//
// Created by leonhard on 31.07.19.
//
#include "var_substitution.h"

std::ostream &wam::operator<<(std::ostream &os, const wam::var_substitution &substitution) {
    os << "[" << substitution.var_name << " / " << substitution.substitute << "]";
    return os;
}
