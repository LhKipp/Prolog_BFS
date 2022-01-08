//
// Created by leonhard on 23.03.20.
//

#ifndef PROLOG_BFS_NODE_BINDING_H
#define PROLOG_BFS_NODE_BINDING_H

#include "wam/compiler/util/node.h"

namespace wam{
    struct node_binding{
    public:
        node var_name, binding;
        node_binding()= default;
        node_binding(node varName, node binding) : var_name(std::move(varName)),
                                                                binding(std::move(binding)) {}
    };
}
#endif //PROLOG_BFS_NODE_BINDING_H
