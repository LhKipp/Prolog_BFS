//
// Created by leonhard on 22.03.20.
//

#include "node_util.h"
#include <vector>
#include <string>
#include <set>
#include "node_iteration.h"


std::vector<wam::var_reg_substitution>
wam::find_var_reg_substitutions(const node &atom) {
    std::vector<wam::var_reg_substitution> result;
    if(atom.is_constant()){
        return result;
    }

    std::set<std::string> handled_vars;
    bfs_order(atom, true, [&](const node *cur_node) {
        if (cur_node->is_variable()) {
            const std::string& name = cur_node->name;
//if var has been added dont add again
            if(handled_vars.find(name) != handled_vars.end()){
                return;
            }
            handled_vars.insert(name);
            if (cur_node->is_permanent()) {
                result.emplace_back(name, cur_node->get_y_reg(), true);
            } else {
                result.emplace_back(name, cur_node->get_x_reg(), false);
            }
        }
    });
    return result;
}

void wam::point_node_to_heap(node &node, const wam::executor& executor) {
    assert(node.is_variable());
    if(node.is_permanent()){
        node.set_heap_index(executor.environments.back().permanent_registers.at(node.get_y_reg()).index);
    }else{
        node.set_heap_index(executor.registers.at(node.get_x_reg()).index);
    }
}

std::vector<const node *> wam::find_vars_in(const node &atom) {
    std::vector<const node *> vars;
    //atom outer is always functor
    bfs_order(atom, true, [&](const node* n){
        if(n->is_variable()){
            vars.push_back(n);
        }
    });
    return vars;
}














