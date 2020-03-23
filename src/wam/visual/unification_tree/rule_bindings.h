//
// Created by leonhard on 19.03.20.
//

#ifndef PROLOG_BFS_RULE_BINDINGS_H
#define PROLOG_BFS_RULE_BINDINGS_H

#include <vector>
#include "../../data/var_binding.h"
#include "wam/compiler/util/node.h"
#include "wam/visual/unification_tree/util/node_var_util.h"
#include <unordered_map>

namespace wam{


    struct rule_bindings{
        std::unordered_map<node, node, node_var_hasher, node_var_equality> found_subs;

        std::vector<node> vars_to_be_resolved;
    };


}
#endif //PROLOG_BFS_RULE_BINDINGS_H
