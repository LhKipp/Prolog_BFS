//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_VAR_BINDING_NODE_H
#define PROLOG_BFS_VAR_BINDING_NODE_H


#include <utility>
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

        /**
         * At the bottom of the tree, the unification either failed, suceeded in a final var_binding of the
         * orig query_vars or continues with a new query_node
         * For each case is one constructor
         */
        std::variant<bool, std::vector<wam::var_binding>, std::unique_ptr<query_node>> child;

    public:
    var_binding_node()= default;

    var_binding_node(const term_code *calledFunctor): called_functor(calledFunctor), child(false){}

    var_binding_node(const term_code *calledFunctor,
                     std::vector<wam::var_binding> varBindings,
                     std::vector<wam::var_binding> final_orig_var_bindings);

    var_binding_node(const term_code *calledFunctor,
                     std::vector<wam::var_binding> varBindings,
                     query_node following_query);


    std::variant<bool, std::vector<wam::var_binding>, std::unique_ptr<query_node>> &getChild(){
        return child;
    }

};
}


#endif //PROLOG_BFS_VAR_BINDING_NODE_H
