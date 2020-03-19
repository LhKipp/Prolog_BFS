//
// Created by leonhard on 08.03.20.
//

#include "unification_tree.h"
#include "../../data/rule.h"
#include "../substitution_util.h"
#include "../../bfs_organizer/data/storage.h"
#include <iterator>
#include "../../config/config.h"

wam::query_node wam::make_tree(const wam::executor &top_exec, const wam::storage& storage) {
    int node_id_counter = 0;
    query_node result = make_tree(top_exec, storage, node_id_counter);
    resolve_query_atoms_name(result, storage);
    return result;
}

wam::query_node wam::make_tree(const wam::executor &top_exec, const wam::storage& storage, int& node_id_counter) {
    using namespace wam;

    const auto& fact_execs = top_exec.get_children();

    query_node result{&top_exec,
                      fact_execs.size(),
                      node_id_counter++};

    std::transform(fact_execs.begin(),
                   fact_execs.end(),
                   result.get_children().begin(),
                   [&](const std::unique_ptr<wam::executor>& exec){

                       var_binding_node result{exec.get(), node_id_counter++};

                       if(exec->succeeded()) {
                           result.set_final_var_bindings(
                                   find_substitutions_from_orig_query(*exec, storage));
                       }else if(exec->is_archived()){// continues
                           result.set_continuing_query(
                                   make_tree(exec->get_last_child(),
                                             storage, node_id_counter));
                       }
                       return result;
                   });

    return result;
}

void wam::resolve_query_atoms_name(wam::query_node &top_query_node, const wam::storage& storage) {
    using namespace wam;
    if(top_query_node.failed()){
        top_query_node.set_resolved_name(top_query_node.get_atom().get_code_info().value);
        return;
    }

    std::stack<rule_bindings> bindings;
    bindings.push({});
    for(var_binding_node& binding_node : top_query_node.get_children()){
        resolve_parent(top_query_node, binding_node, bindings, storage);
    }
}

void wam::resolve_parent(wam::query_node &parent, wam::var_binding_node& binding_node, std::stack<rule_bindings> bindings, const wam::storage& storage) {
    using namespace wam;

    assert(!bindings.empty());
    auto& q_binding_info = bindings.top();
    rule_bindings f_binding_info;

    parent.set_resolved_name(resolve(parent, q_binding_info));

    if(binding_node.failed() || binding_node.is_to_be_continued()){
        return;
    }

    //find all vars in parent
    //get for every var a var_binding
    //find all vars in fact
    //get for every fact var a var_binding
    //Special cases: Q / G_1, F / G_1 --> Q / F, F / Q
    //                    --> query_backtrack_request for Q / F
    //               Q / s(G_1), F / G_1 --> Q / s(F),
    //                    --> query_backtrack_request for Q / s(F)
    //               Q / G_1, F / s(G_1) --> F / s(Q),
    //                 Q gets further executed
    //                e.G. q(s(Q)).?     q(F).
    //                    --> query_backtrack_request for Q (/ G_1)
    // Possible answers:
    //               Q / fact --> only substitute in further query
    //               Q / Var  --> substitute and add var_heap_subst that is to be resolved
    //
    std::vector<std::string> query_vars = find_vars_in(parent.get_query_as_str());
    const auto& all_var_regs = parent.get_atom().get_substitutions();
    std::vector<var_reg_substitution> important_substs;
    important_substs.reserve(all_var_regs.size());
    std::copy_if(all_var_regs.begin(), all_var_regs.end(), std::back_inserter(important_substs),
            [&](const auto& var_reg){
        auto it = std::find(query_vars.begin(), query_vars.end(), var_reg.var_name);
        if(it != query_vars.end()){
            query_vars.erase(it);
            return true;
        }else{
            return false;
        }
    });
    //The rest of the query_vars will be in q_binding_info.q_heap_substs
    std::vector<var_heap_substitution> q_heap_substs = point_var_reg_substs_to_heap(binding_node.get_exec(), important_substs);
    std::copy_if(q_binding_info.heap_substs.begin(), q_binding_info.heap_substs.end(),
                 std::back_inserter(q_heap_substs),
                 [&](const auto& heap_sub){
        return std::find(query_vars.begin(), query_vars.end(), heap_sub.var_name) != query_vars.end();
    });
//    q_binding_info.heap_substs.insert(q_binding_info.heap_substs.end(),
//                                      q_heap_substs.begin(), q_heap_substs.end());

    std::vector<var_heap_substitution> f_heap_substs = point_var_reg_substs_to_heap(binding_node.get_exec());
//    f_binding_info.heap_substs.insert(f_binding_info.heap_substs.end(),
//            f_heap_substs.begin(), f_heap_substs.end());

    auto[all_bindings, fact_bindings_begin] = find_substitutions(
            binding_node.get_exec(),
            storage.functors,
            q_heap_substs,
            f_heap_substs);
    erase_substitutions(
            all_bindings,
            fact_bindings_begin,
            q_binding_info,
            f_binding_info
            );

    binding_node.get_var_bindings() = all_bindings;
    binding_node.get_first_fact_binding_index() = fact_bindings_begin;

    q_binding_info.bindings.insert(q_binding_info.bindings.end(),
            all_bindings.begin() , all_bindings.begin() + fact_bindings_begin);
    f_binding_info.bindings.insert(f_binding_info.bindings.end(),
            all_bindings.begin() + fact_bindings_begin, all_bindings.end());



    //Check whether there are more found all_bindings for this query
//    if(binding_node.succeeded() || binding_node.continues()){
//        auto[query_bindings_begin, query_bindings_end] = binding_node.get_query_bindings();
//        auto v = std::distance(query_bindings_begin, query_bindings_end);
//        q_binding_info.reserve(q_binding_info.size() + v * 2);
//        std::copy(query_bindings_begin, query_bindings_end, std::back_inserter(q_binding_info));
//    }

    if(parent.get_atom().is_last_atom_in_rule()){
        bindings.pop();
    }

    //Look whether we continue downwards
    if(binding_node.failed() || binding_node.is_to_be_continued() || binding_node.succeeded()){
        //No more following queries to be resolved
        return;
    }

    //binding node continues

    if(binding_node.get_atom().get_belonging_rule()->has_body()){
        //This var_binding_node starts a new rule. So insert a new binding vector
        bindings.push(f_binding_info);
    }

    query_node& following_query = binding_node.get_continuing_query();
    if(following_query.failed()){
        //The tree wont progress downwards, so only resolve failing query name
        resolve(following_query, bindings.top());
    }else{
        for(auto& child : following_query.get_children()){
            resolve_parent(following_query, child, bindings, storage);
        }
    }
}

const char* const var_terminators = " ,|)]";
std::string wam::resolve(const wam::query_node &query, const wam::rule_bindings &rule_bindings) {
    const auto& bindings = rule_bindings.bindings;
    const auto& query_name = query.get_atom().get_code_info().value;
    std::string result;
    result.reserve(query_name.size() * 2);

    auto iter = query_name.begin();
    while(iter != query_name.end()){
        if(std::isupper(*iter)){
            //find end of var
            auto var_end = std::find_first_of(iter, query_name.end(),
                                              var_terminators, var_terminators + 5);
            std::string var_name{iter, var_end};
            auto found = std::find_if(bindings.begin(), bindings.end(),[&](const wam::var_binding& var_binding){
                return var_binding.var_name == var_name;
            });
            if(found != bindings.end()){//if there is a substitution
                result += found->binding;
            }else{ //else insert the variable
                result.insert(result.end(), iter, var_end);
            }
            iter = var_end;
        }else{//its not a varname, so just copy
            result += *iter;
            ++iter;
        }
    }
    return result;
}

void wam::remove_repeating_bindings(wam::query_node &node) {

}

std::vector<std::string> wam::find_vars_in(const std::string &query) {
    std::vector<std::string> result;
    auto it = query.begin();
    const auto end = query.end();
    while(it != end){
        if(std::isupper(*it)){
            auto var_end = std::find_first_of(it, end,
                                              var_terminators, var_terminators + 5);
            result.emplace_back(it, var_end);
            it = var_end;
        }else{
            ++it;
        }
    }
    return result;
}

void wam::erase_substitutions(std::vector<var_binding> &all_var_bindings, int &fact_bindings_begin,
                              wam::rule_bindings &q_bindings_info, wam::rule_bindings &f_bindings_info) {


    //Special cases: Q / G_1, F / G_1 --> Q / F, F / Q
    //                    --> query_backtrack_request for Q / F
    //               Q / s(G_1), F / G_1 --> Q / s(F),
    //                    --> query_backtrack_request for Q / s(F)
    //               Q / G_1, F / s(G_1) --> F / s(Q),
    //                 Q gets further executed
    //                e.G. q(s(Q)).?     q(F).
    //                    --> query_backtrack_request for Q (/ G_1)
    using namespace wam;
    //Input size are normaly small, so simple O(n2) algorithm should be performant enough
    //Otherwise alternative algorithm: insert all query_bindings in multi_set
    //Then look for each fact_binding if substitute is in tree, if so rebind fact to one of the query_vars, and make Qvar1/Qvar2

    std::vector<std::vector<var_binding>::iterator> queries_to_erase;
    const auto query_begin = all_var_bindings.begin();
    const auto query_end = all_var_bindings.begin() + fact_bindings_begin;
    const auto fact_begin = all_var_bindings.begin() + fact_bindings_begin;
    const auto fact_end = all_var_bindings.end();

    //For each fact
    std::for_each(fact_begin, fact_end,
            [&](var_binding& fact_binding){

        std::vector<std::vector<var_binding>::iterator> found_queries;
        //Sadly there is no transform_if.
        auto found = std::find_if(query_begin, query_end, [&](auto& q_binding){
            return q_binding.binding == fact_binding.binding && q_binding.binds_to_var();
        });
        while(found != query_end){
            found_queries.push_back(found);
            f_bindings_info.heap_substs.emplace_back
            (found->var_name, std::stoi(found->binding.substr(config::UNBOUND_VAR_PREFIX_SIZE)));

            found = std::find(found + 1, query_end, fact_binding);
        }

        //First if should be common case, second if is unusual.
        //The second if case also handles first if case, but has overhead for common case.
        if(found_queries.size() == 1) {
            //Bind fact_var to first query_binding, and delete query_binding
            fact_binding.binding = found_queries[0]->var_name;
            queries_to_erase.push_back(found_queries[0]);
            //A query_binding is erased, so the facts start earlier
        }else if(found_queries.size() > 1){
            fact_binding.binding = found_queries[0]->var_name;
            //std::transform doesnt guarantee order, so simple implementation here...
            auto bind_queries = [](var_binding& from, const var_binding& to){
                from.binding = to.var_name;
            };
            auto end = found_queries.rend() - 1;//No need to process found_queries[0] as it gets deleted later
            auto first = found_queries.rbegin();
            while(first != end){
                bind_queries(**first, **(++first));
            }

            queries_to_erase.push_back(found_queries[0]);
            //A query_binding is erased, so the facts start earlier
        }
    });

    for(auto to_erase : queries_to_erase){
        all_var_bindings.erase(to_erase);
    }
    fact_bindings_begin -= queries_to_erase.size();

}
