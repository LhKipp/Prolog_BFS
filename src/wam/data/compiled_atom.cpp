//
// Created by leonhard on 17.03.20.
//

#include "prolog_bfs/wam/data/compiled_atom.h"
#include "prolog_bfs/wam/data/rule.h"

bool wam::compiled_atom::is_last_atom_in_rule() const {
    return this == &this->belongs_to->atoms().back();
}
