//
// Created by leonhard on 08.03.20.
//

#ifndef PROLOG_BFS_UNIFICATION_TREE_H
#define PROLOG_BFS_UNIFICATION_TREE_H

#include "../../executor/executor.h"
#include "query_node.h"

namespace wam{

    query_node make_tree(const executor& top_exec, const std::vector<functor_view>& functors);
}

#endif //PROLOG_BFS_UNIFICATION_TREE_H
