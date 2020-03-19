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

namespace wam {
    std::string
    string_representation_of(const executor &executor, size_t index, const std::vector<functor_view> &functors,
            bool is_contigous_list = false);

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
            std::vector<var_binding>,
            int
            >
    find_substitutions(
            const executor& executor,
            const std::vector<functor_view>& functors,
            const std::vector<var_heap_substitution>& var_heap_substs_query,
            const std::vector<var_heap_substitution>& var_heap_subst_func);

    void bind_fact_vars_to_query_vars(std::vector<var_binding>& vector, int& fact_begin);

};
#endif //PROLOG_BFS_SUBSTITUTION_UTIL_H
