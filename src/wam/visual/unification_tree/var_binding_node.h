//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_VAR_BINDING_NODE_H
#define PROLOG_BFS_VAR_BINDING_NODE_H


#include <variant>
#include <memory>
#include "../../data/var_binding.h"
#include "../../data/term_code.h"

namespace wam {
    class query_node;
class var_binding_node {
    private:
        const term_code *called_functor;
        std::vector<wam::var_binding> var_bindings;
        std::variant<wam::var_binding, std::unique_ptr<query_node>> child;

    public:

    };
}


#endif //PROLOG_BFS_VAR_BINDING_NODE_H
