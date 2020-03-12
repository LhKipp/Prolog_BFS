//
// Created by leonhard on 08.03.20.
//

#include "unification_tree.h"
#include "../substitution_util.h"

wam::query_node wam::make_tree(const wam::executor &top_exec, const std::vector<wam::functor_view>& functors) {
    using namespace wam;

    //The top exec is always an query_node
    //The var_heap subs for the query_node stay the same for every fact_exec
    const std::vector<var_heap_substitution> query_var_heap_subs = point_var_reg_substs_to_heap(top_exec);
    const auto& fact_execs = top_exec.get_children();

    query_node result{top_exec.get_solved_term_code(),
                      fact_execs.size()};

    std::transform(fact_execs.begin(),
            fact_execs.end(),
                   result.get_children().begin(),
            [&](const std::unique_ptr<wam::executor>& exec){

                if(exec->is_running()){
                    const auto exec_var_heap_subs = point_var_reg_substs_to_heap(*exec);
                    return var_binding_node{
                        exec->get_current_term_code(),
                        find_substitutions(
                                *exec,
                                functors,
                                query_var_heap_subs,
                                exec_var_heap_subs)
                    };
                }

                term_code* fact_term_code = exec->get_solved_term_code();
                if(exec->failed()){
                    //No more work, just pass the term_code, fail flag set in constructor
                    return var_binding_node{fact_term_code};
                }else if(exec->succeeded()){
                    const auto exec_var_heap_subs = point_var_reg_substs_to_heap(*exec);
                    return var_binding_node{
                            fact_term_code,
                            find_substitutions(
                                    *exec,
                                    functors,
                                    query_var_heap_subs,
                                    exec_var_heap_subs),
                            find_substitutions_from_orig_query(*exec, functors)
                    };
                }else{ //exec has a following query
                    const auto exec_var_heap_subs = point_var_reg_substs_to_heap(*exec);
                    return var_binding_node{fact_term_code,
                                            find_substitutions(
                                                    *exec,
                                                    functors,
                                                    query_var_heap_subs,
                                                    exec_var_heap_subs),
                                            make_tree(exec->get_last_child(),
                                                      functors)};
                }
            });
    return result;
}

