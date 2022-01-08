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
    node point_node_to_heap(const node& node, const executor& exec);

    template<typename node>
    std::vector<node*> find_unique_vars_in(node& atom);

    template<typename node>
    std::unordered_map<std::string, std::vector<node *>> find_all_vars_in(node& atom);
}

template<typename node>
std::vector<node *> wam::find_unique_vars_in(node &atom) {
    std::vector<node *> vars;

    if(atom.is_constant()){
        return vars;
    }
//    std::vector<var_heap_substitution>
    //atom outer is always functor
    bfs_order(atom, true, [&](node* n){
        if(n->is_variable()){
            if(std::find_if(vars.begin(), vars.end(),[&](node* var){
                if(var->name != n->name) return false;
                if(var->is_permanent() && n->is_permanent()){
                    return var->get_y_reg() == n->get_y_reg();
                }else if(!var->is_permanent() && !n->is_permanent()){
                    return var->get_x_reg() == n->get_x_reg();
                }else{
                    return false;
                }
            }) == vars.end()){
                vars.push_back(n);
            }
        }
    });
    return vars;
}

template<typename node>
std::unordered_map<std::string, std::vector<node *>> wam::find_all_vars_in(node &atom) {
    //Within one atom every variable with the same name is the same variable as they all have same x_regs
    std::unordered_map<std::string, std::vector<node *>> result;

    if(atom.is_constant()){
        return result;
    }
    //std::vector<var_heap_substitution>
    //atom outer is always functor
    bfs_order(atom, true, [&](node* n){
        if(n->is_variable()){
            result[n->name].push_back(n);
        }
    });
    return result;
}

#endif //PROLOG_BFS_NODE_UTIL_H
