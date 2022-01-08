//
// Created by leonhard on 29.07.19.
//

#include "prolog_bfs/wam/visual/substitution_util.h"
#include "prolog_bfs/wam/instructions/util/instructions_util.h"
#include "prolog_bfs/wam/visual/unification_tree/util/node_binding.h"
#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/arith_functor.h"


bool is_user_entered_var(
    const std::vector<wam::var_heap_substitution>& user_entered_var_heap_subs,
    const std::string_view var_name){
    return (std::find_if(user_entered_var_heap_subs.begin(), user_entered_var_heap_subs.end(),
                    [&](const auto& var_subst){
                        return var_subst.var_name == var_name;
                    }) != user_entered_var_heap_subs.end());
}


node wam::node_representation_of(const wam::executor &exec, size_t index, const wam::storage &storage) {
    using namespace wam;
    const size_t heap_i = wam::deref(exec, index);
    heap_reg reg = exec.heap_at(heap_i);

    if(reg.is_REF()){
        //Unbound ref cell
        node var_node{STORED_OBJECT_FLAG ::VARIABLE, storage.variables[reg.get_var_i()].name};
        var_node.set_heap_index(heap_i);
        return var_node;
    }

    if(reg.is_INT()){
        //int cell
        node int_node{STORED_OBJECT_FLAG ::INT, std::to_string(reg.get_int_val())};
        return int_node;
    }

    if(reg.is_CONS()){
        const functor_view &functor = storage.functors[reg.get_cons_i()];
        return node{STORED_OBJECT_FLAG ::CONSTANT, functor.name};
    }
    //If the functor is the empty list, we return only "[]" if it is not end-marker for another list.
    // e.g. [a|[]] should be outputed as [a]
    if (reg.is_FUN()) {
        const functor_view &functor = storage.functors[reg.get_fun_i()];
        node func{STORED_OBJECT_FLAG ::FUNCTOR, functor.name};
        for (int i = 0; i < functor.arity ; ++i) {
            func.children->emplace_back(node_representation_of(exec, heap_i + i + 1, storage));
        }
        return func;
    }

    if(reg.is_EVAL_FUN()){
        const functor_view &functor = wam::arithmetic::functor_of(reg.get_eval_fun_i());
        node func{STORED_OBJECT_FLAG ::EVALUABLE_FUNCTOR, functor.name};
        for (int i = 0; i < functor.arity ; ++i) {
            func.children->emplace_back(node_representation_of(exec, heap_i + i + 1, storage));
        }
        return func;
    }

#pragma GCC diagnostic ignored "-Wreturn-type"
    //Assert everything handled
    assert(false);
}
#pragma GCC diagnostic pop

//TODO recursive function, could be optimized through use of stack
std::string
wam::string_representation_of(const executor &executor,
                              size_t index,
                              const wam::storage& storage,
                              const std::vector<wam::var_heap_substitution>& user_entered_var_heap_subs,
        //TODO Better Naming: is_contigous_list should be is_list_start
        //Need to negate part of the logic
                              bool is_contigous_list) {
    using namespace wam;
    auto derefed_index = deref(executor, index);
    heap_reg reg = executor.heap_at(derefed_index);

    std::string var;

    //If the register is still a ref it is an unbound ref cell
    switch(reg.type){
        case heap_tag::REF:
            var = storage.variables[reg.var_index].name;
            // If the index user is interested in represents unbound REF
            // and the REF is not a usere enterd var, append the index 
            // to make the REF unique
            if(index == derefed_index && !is_user_entered_var(user_entered_var_heap_subs, var))
                {var += std::to_string(index);}
            return var;
        case heap_tag::INT:
            return std::to_string(reg.get_int_val());
        case heap_tag::CONS:{
            const functor_view &functor = storage.functors[reg.index];
            return functor.name;
        }
        case heap_tag::FUN: {
            const functor_view &functor = storage.functors[reg.index];
            //If the functor is the empty list, we return only "[]" if it is not end-marker for another list.
            // e.g. [a|[]] should be outputed as [a]
            if (functor.is_empty_list()) {
                if (!is_contigous_list) {
                    return "[]";
                }else{
                    return "";
                }
            }

            if (functor.is_append_functor()) {
                assert(false); //There is no append functor on the heap
//                std::string appended_elem = string_representation_of(executor, index + 1, storage, false);
//                if (appended_elem == "[]") {
//                    //If it only appended the empty list, no need to output it too
//                    return "]";
//                } else {
//                    //TODO is this correct?
//                    return appended_elem + ",]";
//                }
            }
            //Either list or normal functor
            std::string result;

            //If the functor is a list
            if (functor.is_list()) {
                if (!is_contigous_list) {
                    result = "[";
                }
                //Append front element to list
                result += string_representation_of(executor, derefed_index + 1, storage, user_entered_var_heap_subs, false);

                //If rest is variable it is variable list end, which is shown in the output as:
                // |<VarName>
                auto list_rest = derefed_reg(executor, derefed_index + 2);
                if(list_rest.is_REF()){
                    result += '|' + string_representation_of(executor, derefed_index + 2, storage, user_entered_var_heap_subs, true);
                }else{
                    //The list continues with ,Elem ...
                    result += ',' + string_representation_of(executor, derefed_index + 2, storage, user_entered_var_heap_subs, true);
                }

                //If the list has been completly built, replace the last "," with an "]"
                if (!is_contigous_list) {
                    result.back() = ']';
                }
                return result;
            }

            //The functor is a normal functor
            result += functor.name + '(';
            //-1 for correct formatting of the ,
            for (int i = 1; i <= functor.arity - 1; ++i) {
                result += string_representation_of(executor, derefed_index + i, storage, user_entered_var_heap_subs) + ',';
            }
            return result + string_representation_of(executor, derefed_index + functor.arity, storage, user_entered_var_heap_subs) + ")";
        }
        case heap_tag::EVAL_FUN: {
            const auto functor = wam::arithmetic::functor_of(reg.get_eval_fun_i());
            std::string result;
            if(functor.arity == 2){
                return '(' + string_representation_of(executor, derefed_index + 1, storage, user_entered_var_heap_subs)
                + ' ' + functor.name + ' '
                + string_representation_of(executor, derefed_index + 2, storage, user_entered_var_heap_subs) + ')';
            }else if(functor.arity == 1){
                return '(' + functor.name + '('
                + string_representation_of(executor, derefed_index + 1, storage, user_entered_var_heap_subs)
                + "))";
            }else{
                assert(false);
            }
        }
            break;
        case heap_tag::STR:
            //unreachable
            assert(false);
            return "";
    }
    //unreachable
    assert(false);
    return "";
}

std::vector<wam::var_heap_substitution>
wam::point_var_reg_substs_to_heap(const executor &executor ) {
    return point_var_reg_substs_to_heap(
            executor,
            executor.get_cur_or_solved_term_code()->get_substitutions());

}
std::vector<wam::var_heap_substitution> wam::point_var_reg_substs_to_heap(const wam::executor &executor, const std::vector<wam::var_reg_substitution>& var_reg_substs){
#ifdef DEBUG_UNIFICATION_TREE
    std::cout << "point_var_reg_substs_to_heap" << std::endl;
#endif
    std::vector<wam::var_heap_substitution> result{var_reg_substs.size()};
    std::transform(var_reg_substs.begin(),
                   var_reg_substs.end(),
                   result.begin(),
                   [&](const var_reg_substitution& reg_sub){
                       size_t heap_index = reg_sub.is_permanent_register ?
                                           executor.environments.back().permanent_registers.at(reg_sub.register_index).get_heap_i()
                                                                         : executor.registers.at(reg_sub.register_index).get_heap_i();

                       return var_heap_substitution(
                               reg_sub.var_name,
                               heap_index
                       );
                   });

    return result;
}

std::vector<wam::var_binding> wam::find_substitutions_from_orig_query(const wam::executor& executor,
        const wam::storage& storage) {
    //This exec is an empty executor. According to impl of proceed instruction, the father will have
    //unified a term. So we can skip this exec and his father

    std::vector<wam::var_heap_substitution> user_entered_var_heap_subs;
    const wam::executor* parent = &executor.get_parent();
    while(true){
        if(parent->is_from_user_entered_query()){
            auto var_heap_subs = wam::point_var_reg_substs_to_heap(*parent);
            user_entered_var_heap_subs.insert(
                user_entered_var_heap_subs.end(),
                var_heap_subs.begin(),var_heap_subs.end());
        }
        if(!parent->has_parent()){
            break;
        }

        //Its always: query_exec --> fact_exec --> query_exec -->fact_exec ...
        //So we can skip the fact execs
        parent = &parent->get_parent();
        parent = &parent->get_parent();
    }


    /* std::sort(user_entered_var_heap_subs.begin(), user_entered_var_heap_subs.end(), [](const auto& a, const auto& b){return a.var_name == b.var_name;}); */
    /* user_entered_var_heap_subs.erase( */
    /*         std::unique(user_entered_var_heap_subs.begin(), */
    /*                     user_entered_var_heap_subs.end()), */
    /*         user_entered_var_heap_subs.end()); */

    std::vector<wam::var_binding> result;
    for(const auto& var_heap_sub : user_entered_var_heap_subs){
        //If the var has been found in a parent exe already continue
        // TODO This if is necessary
        if(std::find_if(result.begin(), result.end(),
                        [&](const var_binding& var_subst){
                            return var_subst.var_name == var_heap_sub.var_name;
                        }) != result.end()){
            continue;
        }

        result.emplace_back(
                var_heap_sub.var_name,
                wam::string_representation_of(executor, var_heap_sub.heap_index, storage, user_entered_var_heap_subs)
        );
    }
    return result;
}

std::tuple<
        std::vector<wam::node_binding>,
        int
>
wam::find_substitutions(
        const wam::executor &executor,
        const storage& storage,
        const std::vector<node> &var_heap_subst_query,
        const std::vector<node> &var_heap_subst_func) {
    using namespace wam;
    auto transformation = [&](const auto& var_heap_substs, const auto result_begin){
        std::transform(
                var_heap_substs.begin(),
                var_heap_substs.end(),
                result_begin,
                [&](const node& var_heap_node){
                    return node_binding(
                            var_heap_node,
                            node_representation_of(executor, var_heap_node.get_heap_index(), storage));
                });
    };

    std::vector<node_binding>
    result{var_heap_subst_func.size() + var_heap_subst_query.size()};

    int fact_begin = var_heap_subst_query.size();
    transformation(var_heap_subst_query, result.begin());
    transformation(var_heap_subst_func, result.begin() + fact_begin);

    return std::make_tuple(result, fact_begin);
}

