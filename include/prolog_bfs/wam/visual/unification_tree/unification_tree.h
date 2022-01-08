//
// Created by leonhard on 08.03.20.
//

#ifndef PROLOG_BFS_UNIFICATION_TREE_H
#define PROLOG_BFS_UNIFICATION_TREE_H

#include <prolog_bfs/wam/visual/unification_tree/util/node_binding.h>
#include "prolog_bfs/wam/executor/executor.h"
#include "query_node.h"
#include "rule_bindings.h"
#include "prolog_bfs/wam/bfs_organizer/data/storage.h"


namespace wam{

    query_node make_tree(const executor& top_exec, const wam::storage& storage);
    query_node make_tree(const executor& top_exec, const wam::storage& storage, int& id_start);

    void resolve_query_atoms_name(query_node& top_query_node, const wam::storage& storage);
    void resolve_query_node_name(wam::query_node& query_node, const wam::storage& storage);

    void resolve_parent(query_node &parent, var_binding_node &binding_node, const wam::storage& storage);

    void erase_substitutions(std::vector<wam::node_binding>& all_var_bindings,
                             int& fact_bindings_begin);

    void erase_similar(std::vector<node_binding> &all_var_bindings, int &fact_begin);

    void
    erase_var_in_var(std::vector<var_binding> &vector, int &begin, rule_bindings &bindings, rule_bindings &bindings1);
}

#endif //PROLOG_BFS_UNIFICATION_TREE_H
