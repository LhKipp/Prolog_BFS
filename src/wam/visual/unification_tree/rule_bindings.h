//
// Created by leonhard on 19.03.20.
//

#ifndef PROLOG_BFS_RULE_BINDINGS_H
#define PROLOG_BFS_RULE_BINDINGS_H

#include <vector>
#include "../../data/var_binding.h"
#include "../../data/var_heap_substitution.h"

namespace wam{
    struct rule_bindings{
        std::vector<wam::var_binding> bindings;
        std::vector<wam::var_heap_substitution> heap_substs;
    };
}
#endif //PROLOG_BFS_RULE_BINDINGS_H
