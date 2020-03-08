//
// Created by leonhard on 29.07.19.
//
#include "../instructions/util/instructions_util.h"
#include "../config/config.h"

#include "substitution_util.h"

//TODO recursive function, could be optimized through use of stack
std::string
wam::string_representation_of(const executor &executor,
                              size_t index,
                              const std::vector<functor_view> &functors,
        //TODO Better Naming: is_contigous_list should be is_list_start
        //Need to negate part of the logic
                              bool is_contigous_list) {
    //If register is an Ref cell we try to dereference it
    if (executor.heap_at(index).is_REF()) {
        index = wam::deref(executor, executor.heap_at(index));
    }

    //If the register is still a ref it is an unbound ref cell
    if (executor.heap_at(index).is_REF()) {
        return std::string{wam::config::UNBOUND_VAR_PREFIX} + std::to_string(index);
    }

    if (executor.heap_at(index).is_STR()) {
        index = executor.heap_at(index).index;
    }

    //At this point the register will be an FUN cell
    const functor_view &functor = functors[executor.heap_at(index).index];

    //If the functor is the empty list, we return only "[]" if it is not end-marker for another list.
    // e.g. [a|[]] should be outputed as [a]
    if (functor.is_empty_list()) {
        if (!is_contigous_list) {
            return "[]";
        }else{
            return "";
        }
    }

    //If the functor is a constant, we return the name
    if (functor.is_constant()) {
        return functor.name;
    }

    if (functor.is_append_functor()) {
        std::string appended_elem = string_representation_of(executor, index + 1, functors, false);
        if (appended_elem == "[]") {
            //If it only appended the empty list, no need to output it too
            return "]";
        } else {
            //TODO is this correct?
            return appended_elem + ",]";
        }
    }
    //Either list or normal functor
    std::string result;

    //If the functor is a list
    if (functor.is_list()) {
        if (!is_contigous_list) {
            result = "[";
        }
        result += string_representation_of(executor, index + 1, functors, false) + ",";
        result += string_representation_of(executor, index + 2, functors, true);

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
        result += string_representation_of(executor, index + i, functors) + ',';
    }
    return result + string_representation_of(executor, index + functor.arity, functors) + ")";
}

std::vector<wam::var_heap_substitution> wam::point_var_reg_substs_to_heap(const wam::executor &executor){
#ifdef DEBUG_UNIFICATION_TREE
    std::cout << "point_var_reg_substs_to_heap" << std::endl;
#endif
    auto const& var_reg_substs = executor.get_solved_term_code()->get_substitutions();
    std::vector<wam::var_heap_substitution> result{var_reg_substs.size()};

    std::transform(var_reg_substs.begin(),
                   var_reg_substs.end(),
                   result.begin(),
                   [&](const var_reg_substitution& reg_sub){
                       size_t heap_index = reg_sub.is_permanent_register ?
                                           executor.environments.back().permanent_registers.at(reg_sub.register_index).index
                                                                         : executor.registers.at(reg_sub.register_index).index;

                       return var_heap_substitution(
                               reg_sub.var_name,
                               heap_index
                       );
                   });

    return result;
}

std::vector<wam::var_binding> wam::find_substitutions_from_orig_query(const wam::executor& executor,
        const std::vector<functor_view>& functors) {
    std::vector<wam::var_binding> result;
    //normally user have 1 to 5 vars in their queries. so using vector should be more efficient than set

    //This exec is an empty executor. According to impl of proceed instruction, the father will have
    //unified a term. So we can skip this exec and his father

    const wam::executor* parent = &executor.get_parent();
    parent = &parent->get_parent();
    while(true){
        if(parent->is_from_user_entered_query()){
            auto var_heap_subs = wam::point_var_reg_substs_to_heap(*parent);
            for(const auto& var_heap_sub : var_heap_subs){
                if(std::find_if(result.begin(), result.end(),
                                [&](const var_binding& var_subst){
                                    return var_subst.var_name == var_heap_sub.var_name;
                                }) != result.end()){
                    continue;
                }

                result.emplace_back(
                        var_heap_sub.var_name,
                        wam::string_representation_of(executor, var_heap_sub.heap_index, functors)
                );
            }
        }
        if(!parent->has_parent()){
            break;
        }

        //Its always: query_exec --> fact_exec --> query_exec -->fact_exec ...
        //So we can skip the fact execs
        parent = &parent->get_parent();
        parent = &parent->get_parent();
    }
    return result;
}

std::vector<wam::var_binding>
wam::find_substitutions(
        const wam::executor &executor,
        const std::vector<functor_view> &functors,
        const std::vector<var_heap_substitution> &var_heap_subst_query,
        const std::vector<var_heap_substitution> &var_heap_subst_func) {
    using namespace wam;
    std::vector<var_binding> result{var_heap_subst_func.size() + var_heap_subst_query.max_size()};
    auto transformation = [&](const auto& var_heap_substs, const auto result_begin){
        std::transform(
                var_heap_substs.begin(),
                var_heap_substs.end(),
                result_begin,
                [&](const auto& var_heap_sub){
                    return var_binding{
                        var_heap_sub.var_name,
                                wam::string_representation_of(executor, var_heap_sub.heap_index, functors)
                    };
                });
    };

    transformation(var_heap_subst_query, result.begin());
    transformation(var_heap_subst_func, result.begin() + var_heap_subst_query.size());

    return result;
}

