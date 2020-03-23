//
// Created by leonhard on 17.03.20.
//

#include "compiled_atom.h"
#include "rule.h"

bool wam::compiled_atom::is_last_atom_in_rule() const {
    return this == &this->belongs_to->atoms().back();
}
