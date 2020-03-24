//
// Created by leonhard on 22.03.20.
//

#include "node_util.h"
#include <vector>
#include <string>
#include <set>
#include <wam/visual/unification_tree/util/node_var_util.h>
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

void wam::point_node_to_heap(node &n, const wam::executor& executor) {
    assert(n.is_variable());
    if(n.is_permanent()){
        assert(executor.environments.back().permanent_registers.size() > n.get_y_reg());
        n.set_heap_index(executor.environments.back().permanent_registers.at(n.get_y_reg()).index);
    }else{
        assert(executor.registers.size() > n.get_x_reg());
        n.set_heap_index(executor.registers.at(n.get_x_reg()).index);
    }
}
node wam::point_node_to_heap(const node &n, const wam::executor& executor) {
    assert(n.is_variable());
    node copy{n};
    if(n.is_permanent()){
        copy.set_heap_index(executor.environments.back().permanent_registers.at(n.get_y_reg()).index);
    }else{
        copy.set_heap_index(executor.registers.at(n.get_x_reg()).index);
    }
    return copy;
}















