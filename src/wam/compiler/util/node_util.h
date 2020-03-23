//
// Created by leonhard on 22.03.20.
//

#ifndef PROLOG_BFS_NODE_UTIL_H
#define PROLOG_BFS_NODE_UTIL_H

#include "wam/executor/executor.h"
#include "node.h"
#include "../../data/var_reg_substitution.h"

/*
 * Finds and lists the variable name to register substitutions
 */
namespace wam{

    std::vector<wam::var_reg_substitution>
    find_var_reg_substitutions(const node &atom);

    void point_node_to_heap(node& node, const executor& exec);

    std::vector<const node*> find_vars_in(const node& atom);

}
#endif //PROLOG_BFS_NODE_UTIL_H
