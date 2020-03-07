//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_QUERY_NODE_H
#define PROLOG_BFS_QUERY_NODE_H

#include "var_binding_node.h"
#include "../../data/term_code.h"
#include <memory>

namespace wam {
    class query_node{
    private:
        const term_code *query;
        std::unique_ptr<std::vector<var_binding_node>> children;
    };
}

#endif //PROLOG_BFS_QUERY_NODE_H
