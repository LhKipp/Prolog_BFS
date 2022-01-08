//
// Created by leonhard on 08.03.20.
//

#include "prolog_bfs/wam/visual/unification_tree/unification_tree.h"
#include "prolog_bfs/wam/data/rule.h"

#include "prolog_bfs/wam/visual/substitution_util.h"
#include "prolog_bfs/wam/bfs_organizer/data/storage.h"


#include <prolog_bfs/wam/compiler/util/node_iteration.h>
#include <prolog_bfs/wam/compiler/util/node_util.h>

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

/*
 * The query exec must not be running so that the reg.heap_i can be retrieved
 */
void wam::resolve_query_node_name(wam::query_node& query_node, const wam::storage& storage){
    if(query_node.is_to_be_continued()){
        query_node.set_name(node{STORED_OBJECT_FLAG ::CONSTANT, "cant determine name yet"});
        return;
    }
    assert(!query_node.is_to_be_continued());
    node query_name = query_node.get_atom().get_parsed_atom();
    //atom outer is always functor

    std::unordered_map<std::string, std::vector<node *>> vars = find_all_vars_in(query_name);
    for(auto& var_vec : vars){
        node pointed_to_heap = point_node_to_heap((const node&) *var_vec.second[0], query_node.get_exec());
        node var_representation = node_representation_of(query_node.get_exec(), pointed_to_heap.get_heap_index(), storage);
        for(node* var : var_vec.second){
            *var = var_representation;
        }
    }
    query_node.set_name(query_name);
};

void wam::resolve_query_atoms_name(wam::query_node &top_query_node, const wam::storage& storage) {
    using namespace wam;

    //The top query node name doesnt change
    resolve_query_node_name(top_query_node, storage);

    if(top_query_node.failed() || top_query_node.is_to_be_continued()){
        return;
    }

    for(var_binding_node& binding_node : top_query_node.get_children()){
        if(binding_node.failed() || binding_node.is_to_be_continued()){
            continue;
        }
        resolve_parent(top_query_node, binding_node,  storage);
    }
}

void wam::resolve_parent(wam::query_node &parent, wam::var_binding_node& binding_node, const wam::storage& storage) {
    using namespace wam;

    assert(!(parent.is_to_be_continued() || parent.failed()));
    assert(!(binding_node.failed() || binding_node.is_to_be_continued()));

    std::vector<const node*> f_vars = find_unique_vars_in(binding_node.get_atom().get_parsed_atom());
    std::vector<const node*> q_vars = find_unique_vars_in(parent.get_name());

    std::vector<node> f_heap_vars{f_vars.size()};
    std::transform(f_vars.begin(), f_vars.end(),
            f_heap_vars.begin(),
            [&](const node* n){
        return point_node_to_heap(*n, binding_node.get_exec());
    });

    std::vector<node> q_heap_vars{q_vars.size()};
    std::transform(q_vars.begin(), q_vars.end(),
                   q_heap_vars.begin(),
                   [&](const node* n){
                       return *n;
                   });

    auto[all_bindings, fact_bindings_begin] = find_substitutions(
            binding_node.get_exec(),
            storage,
            q_heap_vars,
            f_heap_vars);
    erase_substitutions(
            all_bindings,
            fact_bindings_begin
    );

    assert(fact_bindings_begin <= all_bindings.size());

    std::vector<var_binding> bindgs_as_var_bindgs{all_bindings.size()};
    std::transform(all_bindings.begin(), all_bindings.end(),
                   bindgs_as_var_bindgs.begin(),
                   [](const node_binding& binding){
        return var_binding{binding.var_name.to_string(), binding.binding.to_string()};
    });
    binding_node.get_var_bindings() = std::move(bindgs_as_var_bindgs);
    binding_node.get_first_fact_binding_index() = fact_bindings_begin;

    //Look whether we continue downwards
    if(binding_node.failed() || binding_node.is_to_be_continued() || binding_node.succeeded()){
        //No more following queries to be resolved
        return;
    }

    //binding node continues
    query_node& following_query = binding_node.get_continuing_query();

    resolve_query_node_name(following_query, storage);

    if(following_query.failed() || following_query.is_to_be_continued()){
        return;
    }

    for(auto& child : following_query.get_children()){
        if(child.failed() || child.is_to_be_continued()){
            continue;
        }
        resolve_parent(following_query, child, storage);
    }
}


void wam::erase_substitutions(std::vector<wam::node_binding> &all_var_bindings,
                              int &fact_bindings_begin){
    using namespace wam;

    const auto query_begin = all_var_bindings.begin();
    const auto query_end = all_var_bindings.begin() + fact_bindings_begin;
    const auto fact_begin = all_var_bindings.begin() + fact_bindings_begin;
    const auto fact_end = all_var_bindings.end();

    erase_similar(all_var_bindings,
                  fact_bindings_begin);

}

void
wam::erase_similar(std::vector<node_binding> &all_var_bindings,
                   int &fact_bindings_begin){

    const auto query_begin = all_var_bindings.begin();
    const auto query_end = all_var_bindings.begin() + fact_bindings_begin;

    //Remove all query vars pointing to themselves
    auto q_end_new = std::remove_if(query_begin, query_end, [&](const auto& bindg){
        return bindg.var_name.is_variable() && bindg.binding.is_variable() &&
                node_var_equality{}(bindg.var_name, bindg.binding);
    });

    //remove node_bindings from vec
    fact_bindings_begin = std::distance(query_begin, q_end_new);
    all_var_bindings.erase(q_end_new, query_end);


    const auto fact_begin = all_var_bindings.begin() + fact_bindings_begin;
    const auto fact_end = all_var_bindings.end();
    auto f_end_new = std::remove_if(fact_begin, fact_end, [&](const auto& bindg){
        return bindg.var_name.is_variable() && bindg.binding.is_variable() &&
               node_var_equality{}(bindg.var_name, bindg.binding);
    });
    all_var_bindings.erase(f_end_new, all_var_bindings.end());
}
