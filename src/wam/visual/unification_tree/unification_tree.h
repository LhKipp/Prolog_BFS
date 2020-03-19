//
// Created by leonhard on 08.03.20.
//

#ifndef PROLOG_BFS_UNIFICATION_TREE_H
#define PROLOG_BFS_UNIFICATION_TREE_H

#include "../../executor/executor.h"
#include "query_node.h"
#include "rule_bindings.h"
#include "../../bfs_organizer/data/storage.h"


namespace wam{

    query_node make_tree(const executor& top_exec, const wam::storage& storage);
    query_node make_tree(const executor& top_exec, const wam::storage& storage, int& id_start);

    void resolve_query_atoms_name(query_node& top_query_node, const wam::storage& storage);

    void resolve_parent(query_node &parent, var_binding_node &binding_node, std::stack<wam::rule_bindings> bindings, const wam::storage& storage);

    std::string resolve(const query_node &query,const wam::rule_bindings &bindings);

    void remove_repeating_bindings(query_node &node);

    std::vector<std::string> find_vars_in(const std::string &query);

    void erase_substitutions(std::vector<var_binding>& all_var_bindings, int& fact_bindings_begin,
                        rule_bindings &q_bindings_info,
                        rule_bindings &f_bindings_info);

}

#endif //PROLOG_BFS_UNIFICATION_TREE_H
