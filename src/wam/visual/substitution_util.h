//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_SUBSTITUTION_UTIL_H
#define PROLOG_BFS_SUBSTITUTION_UTIL_H

#include "../executor/executor.h"
#include "../data/functor_view.h"
#include "../data/var_binding.h"
#include "../bfs_organizer/data/storage.h"
#include <string>
#include <wam/visual/unification_tree/util/node_binding.h>

namespace wam {
    std::string
    string_representation_of(const executor &executor, size_t index, const storage& storage,
            const std::vector<wam::var_heap_substitution>& user_entered_var_heap_subs,
            bool is_contigous_list = false);

    node node_representation_of(const executor& exec, size_t index, const storage& storage);

    std::vector<var_heap_substitution>
    point_var_reg_substs_to_heap(const executor &executor, const std::vector<wam::var_reg_substitution>& var_reg_substs);

    std::vector<var_heap_substitution>
    point_var_reg_substs_to_heap(const executor &executor);

    /**
     * Finds all substitutions for variables from original user entered query atoms in the give executor
     * @param executor - the executor in whoes heap the final substitutions are to find
     * @return
     */
    std::vector<var_binding> find_substitutions_from_orig_query(
            const executor& executor,
            const wam::storage& functors);

    std::tuple<
            std::vector<wam::node_binding>,
            int
            >
    find_substitutions(
            const wam::executor &executor,
            const storage& storage,
            const std::vector<node> &var_heap_subst_query,
            const std::vector<node> &var_heap_subst_func);

    std::vector<std::string> find_vars_in(const std::string &query);
    bool resolve(std::string& statement, const std::vector<var_binding> &bindings);

};
#endif //PROLOG_BFS_SUBSTITUTION_UTIL_H
