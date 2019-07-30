//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_UTIL_H
#define PROLOG_BFS_SUBSTITUTION_UTIL_H

#include "../../data/var_substitution.h"
#include "../../data/var_reg_substitution.h"
#include "../../executor/executor.h"

namespace wam {
    using var_substitutions = std::vector<var_substitution>;
    using var_reg_substitutions = std::vector<var_reg_substitution>;

    var_substitution substitution_for(const var_reg_substitution &var_reg_sub, const executor &executor);

    var_substitutions substitutions_for(const var_reg_substitutions& substitutions, const executor &executor);

};
#endif //PROLOG_BFS_SUBSTITUTION_UTIL_H
