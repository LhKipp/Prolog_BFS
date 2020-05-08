//
// Created by leonhard on 29.07.19.
//
#include "../instructions/util/instructions_util.h"

#include "substitution_util.h"
#include "../bfs_organizer/data/storage.h"
#include "wam/visual/unification_tree/util/node_binding.h"


node wam::node_representation_of(const wam::executor &exec, size_t index, const wam::storage &storage) {
    using namespace wam;
    regist reg = exec.heap_at((index));
    if(reg.is_REF()){
        index = wam::deref(exec, reg);
    }

    reg = exec.heap_at((index));
    if(reg.is_REF()){
        //Unbound ref cell
        node var_node{STORED_OBJECT_FLAG ::VARIABLE, storage.variables[reg.var_index].name};
        var_node.set_heap_index(index);
        return var_node;
    }

    if(reg.is_INT()){
        //int cell
        node int_node{STORED_OBJECT_FLAG ::INT, std::to_string(reg.get_int_val())};
        return int_node;
    }

    if (reg.is_STR()) {
        index = reg.index;
    }

    //At this point the register will be an FUN cell
    reg = exec.heap_at(index);
    const functor_view &functor = storage.functors[reg.index];

    if(functor.is_constant()){
        return node{STORED_OBJECT_FLAG ::CONSTANT, functor.name};
    }
    //If the functor is the empty list, we return only "[]" if it is not end-marker for another list.
    // e.g. [a|[]] should be outputed as [a]
    if (functor.is_empty_list()) {
        return node{STORED_OBJECT_FLAG ::FUNCTOR, "["};
    }

    if (functor.is_append_functor()) {
        node append_func{STORED_OBJECT_FLAG ::FUNCTOR, "|"};
        append_func.children->emplace_back(node_representation_of(exec, index + 1, storage));
    }

    //If the functor is a list
    if (functor.is_list()) {
        node list{STORED_OBJECT_FLAG ::FUNCTOR, "["};
        list.children->emplace_back(node_representation_of(exec, index + 1, storage));
        list.children->emplace_back(node_representation_of(exec, index + 2, storage));
        return list;
    }

    //The functor is a normal functor
    node func_node{STORED_OBJECT_FLAG ::FUNCTOR, functor.name};
    for (int i = 1; i <= functor.arity ; ++i) {
        func_node.children->emplace_back(node_representation_of(exec, index + i, storage));
    }
    return func_node;
}

//TODO recursive function, could be optimized through use of stack
std::string
wam::string_representation_of(const executor &executor,
                              size_t index,
                              const wam::storage& storage,
        //TODO Better Naming: is_contigous_list should be is_list_start
        //Need to negate part of the logic
                              bool is_contigous_list) {
    using namespace wam;
    //If register is an Ref cell we try to dereference it
    regist reg = executor.heap_at(index);
    if (reg.is_REF()) {
        index = wam::deref(executor, reg);
    }

    reg = executor.heap_at(index);

    //If the register is still a ref it is an unbound ref cell
    if (reg.is_REF()) {
        return storage.variables[reg.var_index].name;
    }

    if (reg.is_INT()){
        return std::to_string(reg.get_int_val());
    }

    if (reg.is_STR()) {
        index = reg.index;
    }

    //At this point the register will be an FUN cell
    const functor_view &functor = storage.functors[executor.heap_at(index).index];

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
        assert(false); //There is no append functor on the heap
        std::string appended_elem = string_representation_of(executor, index + 1, storage, false);
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
        result += string_representation_of(executor, index + 1, storage, false) + ",";
        result += string_representation_of(executor, index + 2, storage, true);

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
        result += string_representation_of(executor, index + i, storage) + ',';
    }
    return result + string_representation_of(executor, index + functor.arity, storage) + ")";
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
        const wam::storage& storage) {
    std::vector<wam::var_binding> result;
    //normally user have 1 to 5 vars in their queries. so using vector should be more efficient than set

    //This exec is an empty executor. According to impl of proceed instruction, the father will have
    //unified a term. So we can skip this exec and his father

    const wam::executor* parent = &executor.get_parent();
    while(true){
        if(parent->is_from_user_entered_query()){
            auto var_heap_subs = wam::point_var_reg_substs_to_heap(*parent);
            for(const auto& var_heap_sub : var_heap_subs){
                //If the var has been found in a parent exe already continue
                if(std::find_if(result.begin(), result.end(),
                                [&](const var_binding& var_subst){
                                    return var_subst.var_name == var_heap_sub.var_name;
                                }) != result.end()){
                    continue;
                }

                result.emplace_back(
                        var_heap_sub.var_name,
                        wam::string_representation_of(executor, var_heap_sub.heap_index, storage)
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

