//
// Created by leonhard on 31.07.19.
//
#include "var_binding.h"

std::ostream &wam::operator<<(std::ostream &os, const wam::var_binding &substitution) {
    os << "[" << substitution.var_name << " / " << substitution.binding << "]";
    return os;
}

bool wam::var_binding::operator==(const wam::var_binding &rhs) const {
    return var_name == rhs.var_name &&
           binding == rhs.binding;
}

bool wam::var_binding::operator!=(const wam::var_binding &rhs) const {
    return !(rhs == *this);
}
