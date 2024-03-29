//
// Created by leonhard on 05.07.19.
//

#include "prolog_bfs/wam/compiler/compiler.h"
#include <unordered_map>
#include <numeric>
#include "prolog_bfs/wam/compiler/util/node_iteration.h"
#include "prolog_bfs/wam/compiler/util/node_util.h"

#include "prolog_bfs/wam/instructions/instructions.h"
#include "prolog_bfs/wam/compiler/util/seen_register.h"

#include "prolog_bfs/wam/compiler/parser/parser.h"
#include "prolog_bfs/wam/data/rule.h"
#include "prolog_bfs/wam/bfs_organizer/data/storage.h"

#include "prolog_bfs/wam/compiler/built_in_pred_comp.h"
#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/arith_functor.h"
#include "prolog_bfs/wam/compiler/checks/undefined_var.h"
#include "prolog_bfs/wam/compiler/checks/redefinition_of_built_in_pred.h"

/*
 * Assigns register to an functor (constant is also viable)
 * Returns a tuple (count of registers, count of functors/constants)
 */
int wam::assign_registers(node &functor, node *first_body_atom) {
    if (first_body_atom && first_body_atom->is_constant()) {
        first_body_atom->set_x_reg(0);
        first_body_atom->set_a_reg(0);
        return 1 + assign_registers(functor);
    }
    if (functor.is_constant()) {
        functor.set_x_reg(0);
        functor.set_a_reg(0);

        if (first_body_atom) {
            return 1 + assign_registers(*first_body_atom);
        } else {
            return 1;
        }
    }

    //Both are normal functors

    //first we assign the arguments a_registers
    size_t reg_index = 0;
    size_t first_body_atom_reg_index = 0;
    for (auto &node : *functor.children) {
        node.set_a_reg(reg_index);
        node.set_x_reg(reg_index);

        ++reg_index;
    }
    if (first_body_atom) {
        for (auto &node : *(first_body_atom->children)) {
            node.set_a_reg(first_body_atom_reg_index);
            node.set_x_reg(first_body_atom_reg_index);
            ++first_body_atom_reg_index;
        }
    }

    //Now we assign the x_registers

    //variable name --> first occurence of var
    std::unordered_map<std::string, node *> first_occurences;

    auto assign_var_x_reg = [&](node *node, size_t &reg_counter) {
        if (first_occurences[node->name]) {//if the var has already been seen
            node->set_x_reg(first_occurences[node->name]->get_x_reg());
        } else {
            node->set_x_reg(reg_counter);
            first_occurences[node->name] = node;
            ++reg_counter;
        }
    };

    auto assign_term_x_regs = [&](node &outer_functor, size_t &reg_counter) {
        //skip functor = true
        //functor is outer node and does not need an register
        bfs_order(outer_functor, true, [&](node *node) {
            //If The node is an argument it has an a_reg
            //Only arguments that are vars will also be assigned an x_reg
            if (node->is_argument()) {
                if (node->is_variable()) {
                    assign_var_x_reg(node, reg_counter);
                    return;
                } else {
                    //The node is an constant or outer_functor and has an a_reg
                    return;
                }
            }

            //The node is not an argument
            if (node->is_variable()) {
                assign_var_x_reg(node, reg_counter);
            } else {//constant or outer_functor
                node->set_x_reg(reg_counter);
                ++reg_counter;
            }
        });
    };

    assign_term_x_regs(functor, reg_index);
    if (first_body_atom) {
        assign_term_x_regs(*first_body_atom, first_body_atom_reg_index);
    }
    return std::max(reg_index, first_body_atom_reg_index);
}

/*
 * returns a flattened form of the term (e.g. for generating instructions).
 *
 * Note: The outer functor will be the first element in the returned vector
 */
std::vector<const node *> wam::flatten_program(const node &outer_functor) {
    std::vector<const node *> result;

    if(outer_functor.is_constant()){
        result.push_back(&outer_functor);
        return result;
    }
    bfs_order(outer_functor, true, [&](const node *node) {
        if (node->is_functor()
            || node->is_evaluable_functor()
            || node->is_argument()) {
            result.push_back(node);
        }
    });
    return result;
}


template<typename OutputIter>
void
wam::to_query_instructions(const std::vector<const node *> &flattened_term,
                           const node &outer_functor,
                           OutputIter out,
                           std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                           storage& storage) {
    using namespace std::placeholders;
    using namespace wam::helper;

    std::for_each(flattened_term.begin(), flattened_term.end(), [&](const node *node) {
        if (node->is_variable()) {//Every node in flattened term, beeing an variable is an argument!
            const seen_register vars_reg{register_type::X_REG, node->get_x_reg()};

            //Check if this is an permanent variable yet encountered
            if (node->is_permanent()) {
                const seen_register vars_perm_reg{register_type::Y_REG, node->get_y_reg()};
                if (seen_registers[vars_perm_reg]) {
                    *out = std::bind(wam::put_permanent_value, _1, node->get_y_reg(), node->get_a_reg());
                    ++out;
                } else {
                    //Also put in that we encountered the permanent var
                    seen_registers[vars_perm_reg] = true;
                    //Shouldnt be needed
//                    seen_registers[vars_reg] = true;

                    short var_index = storage.push_back_var(node->name);
                    *out = std::bind(wam::put_permanent_variable, _1, node->get_y_reg(), node->get_a_reg(), var_index);
                    ++out;
                }
            } else {//node is not a perm variable
                if (seen_registers[vars_reg]) {
                    *out = std::bind(wam::put_value, _1, node->get_x_reg(), node->get_a_reg());
                    ++out;
                } else {
                    seen_registers[vars_reg] = true;

                    short var_index = storage.push_back_var(node->name);
                    *out = std::bind(wam::put_variable, _1, node->get_x_reg(), node->get_a_reg(), var_index);
                    ++out;
                }
            }
            return;
        }
        //The node is a functor or constant or int or eval_func
        //Shouldnt be needed
        const seen_register func_reg{register_type::X_REG, node->get_x_reg()};
        seen_registers[func_reg] = true;


        if(node->is_int()){
            *out = std::bind(wam::put_structure, _1,
                             heap_reg{heap_tag::INT, std::stoi(node->name), 0},
                             node->get_x_reg());
            ++out;
            return;
        }else if(node->is_constant()){
            const functor_view functor_view = node->to_functor_view();
            *out = std::bind(wam::put_structure, _1,
                             heap_reg{heap_tag::CONS, storage.functor_index_of(functor_view), 0},
                             node->get_x_reg());
            ++out;
            return;
        }else if(node->is_evaluable_functor()){
            compiler::check_and_throw_undefined_var(outer_functor, *node, seen_registers);
            const int arith_func_i = wam::arithmetic::to_index(node->name);
            const int arity = wam::arithmetic::arity_of(arith_func_i);
            *out = std::bind(wam::put_structure, _1,
                             heap_reg{heap_tag::EVAL_FUN, arith_func_i, (short)arity},
                             node->get_x_reg());
            ++out;
        }else{
            const functor_view functor_view = node->to_functor_view();
            const int func_i = storage.functor_index_of(functor_view);
            *out = std::bind(wam::put_structure, _1,
                             heap_reg{heap_tag::FUN, func_i, (short)functor_view.arity},
                             node->get_x_reg());
            ++out;
        }

        for (auto &childs : *node->children) {
            if(childs.is_int()){
                *out = std::bind(wam::put_structure, _1,
                                 heap_reg{heap_tag::INT, std::stoi(childs.name),0},
                                 childs.get_x_reg());
                ++out;
                continue;
            }else if(childs.is_constant()){
                const functor_view functor_view = childs.to_functor_view();
                *out = std::bind(wam::put_structure, _1,
                                 heap_reg{heap_tag::CONS, storage.functor_index_of(functor_view), 0},
                                 childs.get_x_reg());
                ++out;
                continue;
            }
            const seen_register child_reg{register_type::X_REG, childs.get_x_reg()};

            if (childs.is_permanent()) {
                const seen_register child_perm_reg{register_type::Y_REG, childs.get_y_reg()};
                if (seen_registers[child_perm_reg]) {
                    *out = std::bind(wam::set_permanent_value, _1, childs.get_y_reg());
                    ++out;
                } else {
                    seen_registers[child_perm_reg] = true;
                    //Shouldnt be needed
//                    seen_registers[child_reg]=true;

                    short var_index = storage.push_back_var(childs.name);
                    *out = std::bind(wam::set_permanent_variable, _1, childs.get_y_reg(), var_index);
                    ++out;
                }
            } else {
                if (seen_registers[child_reg]) {
                    *out = std::bind(wam::set_value, _1, childs.get_x_reg());
                    ++out;
                } else {
                    seen_registers[child_reg] = true;
                    short var_index = storage.push_back_var(childs.name);
                    *out = std::bind(wam::set_variable, _1, childs.get_x_reg(), var_index);
                    ++out;
                }
            }
        }
    });

    *out = std::bind(wam::call, _1, outer_functor.to_functor_view());
    ++out;
}

template<typename OutputIter>
void
wam::to_program_instructions(const std::vector<const node *> &flattened_term,
                             OutputIter out,
                             std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                             wam::storage& storage) {
    using namespace std::placeholders;
    using namespace wam::helper;
    //TODO isnt a set enough?

    std::for_each(flattened_term.begin(), flattened_term.end(), [&](const node *node) {
        if (node->is_variable()) {//If the node is an variable argument
            const seen_register vars_reg{register_type::X_REG, node->get_x_reg()};

            if (seen_registers[vars_reg]) {//If the var has been seen
                if (node->is_permanent()) {
                    *out = std::bind(wam::get_permanent_value, _1, node->get_y_reg(), node->get_a_reg());
                    ++out;
                } else {
                    *out = std::bind(wam::get_value, _1, node->get_x_reg(), node->get_a_reg());
                    ++out;
                }
            } else { //If the var hasn't been seen
                seen_registers[vars_reg] = true;
                if (node->is_permanent()) {
                    //Also put in that we encountered the permanent var
                    const seen_register var_perm_reg{register_type::Y_REG, node->get_y_reg()};
                    seen_registers[var_perm_reg] = true;

                    *out = std::bind(wam::get_permanent_variable, _1, node->get_y_reg(), node->get_a_reg());
                    ++out;
                } else {
                    *out = std::bind(wam::get_variable, _1, node->get_x_reg(), node->get_a_reg());
                    ++out;
                }
            }
            //It is a var, no need to process children
            return;
        }

        //Not needed because in a program term, an outer functor wont repeat as an inner functor again
//        const seen_register func_reg{register_type::X_REG, node->get_x_reg()};
//        seen_registers[func_reg] = true;

        if(node->is_int()){
            *out = std::bind(wam::get_structure, _1,
                             heap_reg{heap_tag::INT, std::stoi(node->name), 0},
                             node->get_x_reg());
            ++out;
            return;
        }else if(node->is_evaluable_functor()){
            const int arith_func_i = wam::arithmetic::to_index(node->name);
            const int arity = wam::arithmetic::arity_of(arith_func_i);
            *out = std::bind(wam::get_structure, _1,
                             heap_reg{heap_tag::EVAL_FUN, arith_func_i, (short)arity},
                             node->get_x_reg());
            ++out;
        }else if(node->is_functor()){//func or cons
            const functor_view functor_view = node->to_functor_view();
            *out = std::bind(wam::get_structure, _1,
                             heap_reg{heap_tag::FUN, storage.functor_index_of(functor_view), (short) functor_view.arity},
                             node->get_x_reg());
            ++out;
        }else if(node->is_constant()){
            const functor_view functor_view = node->to_functor_view();
            *out = std::bind(wam::get_structure, _1,
                             heap_reg{heap_tag::CONS, storage.functor_index_of(functor_view), 0},
                             node->get_x_reg());
            ++out;
            return;
        }

        //If node is functor
        for (auto &childs : *node->children) {
            if(childs.is_int()){
                *out = std::bind(wam::unify_structure, _1,
                                 heap_reg{heap_tag::INT, std::stoi(childs.name), 0},
                                 childs.get_x_reg());
                ++out;
                continue;
            }else if(childs.is_constant()){
                const auto& fn_view = childs.to_functor_view();
                *out = std::bind(wam::unify_structure, _1,
                                 heap_reg{heap_tag::CONS, storage.functor_index_of(fn_view), 0},
                                 childs.get_x_reg());
                ++out;
                continue;
            }
            //else its a var

            const seen_register child_reg{register_type::X_REG, childs.get_x_reg()};
            if (seen_registers[child_reg]) {
                if (childs.is_permanent()) {
                    //If the child is permanent also put in that we encountered the Y_REG
                    *out = std::bind(wam::unify_permanent_value, _1, childs.get_y_reg());
                    ++out;
                } else {
                    *out = std::bind(wam::unify_value, _1, childs.get_x_reg());
                    ++out;
                }
            } else {//if not encountered yet
                seen_registers[child_reg] = true;

                short var_index = storage.push_back_var(childs.name);
                if (childs.is_permanent()) {
                    const seen_register child_perm_reg{register_type::Y_REG, childs.get_y_reg()};
                    seen_registers[child_perm_reg] = true;
                    *out = std::bind(wam::unify_permanent_variable, _1, childs.get_y_reg(), var_index);
                    ++out;
                } else {
                    *out = std::bind(wam::unify_variable, _1, childs.get_x_reg(), var_index);
                    ++out;
                }
            }
        }
    });

    *out = std::bind(wam::proceed, _1);
    ++out;
}

void remove_x_a_regs(std::unordered_map<wam::helper::seen_register, bool> &seen_registers) {
    for (auto it = begin(seen_registers); it != end(seen_registers);) {
        if (it->first.is_a_reg() || it->first.is_x_reg()) {
            it = seen_registers.erase(it);
        } else
            ++it;
    }
}

wam::rule wam::compile_query(const std::string_view query_code, wam::storage& storage) {
    using namespace std::placeholders;
    using wam::helper::seen_register;

    node query_nodes; //= parser result
    wam::parse_query(query_code, query_nodes);

    const auto permanent_count = assign_permanent_registers(*query_nodes.children, false);

    std::unordered_map<seen_register, bool> seen_registers;
    //Generate the Instructions
    //Instruction container
    std::vector<std::function<void(wam::executor &)>> instructions;

    //The first iteration has a allocate instruction, if there are permanent registers
    if (permanent_count) {
        instructions.emplace_back(std::bind(wam::allocate, _1, permanent_count));
    }

    wam::rule query;
    for (auto & atom : *query_nodes.children) {
        compile_query_atom(std::move(atom), seen_registers, instructions, query, true, storage);
    }

    //LOL Real brogrammes dont free bro :-)
//    if (permanent_count) {
//        instructions.emplace_back(std::bind(wam::deallocate, _1));
//        term_codes.emplace_back(
//                0,
//                instructions,
//                std::vector<var_reg_substitution>{});
//    }

    return query;
}

void wam::compile_query_atom(node &&atom, std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                             std::vector<std::function<void(wam::executor &)>> &instructions,
                             wam::rule &rule,
                             bool from_original_query,
                             storage& storage) {
    const int x_a_reg_counts = assign_registers(atom);

    const std::vector<const node *> flattened_form = flatten_query(atom);
    to_query_instructions(flattened_form,
                          atom,
                          std::back_inserter(instructions),
                          seen_registers,
                          storage);

    rule.add_atom(x_a_reg_counts,
                            instructions,
                            find_var_reg_substitutions(atom),
                            atom.code_info,
                            std::move(atom),
                            from_original_query);

    //Clear the instructions generated so far
    instructions.clear();
    remove_x_a_regs(seen_registers);
}

std::unordered_map<wam::functor_view, std::vector<wam::rule>>
wam::compile_program(const std::string_view program_code, wam::storage& storage){
    using namespace std::placeholders;
    namespace qi = boost::spirit::qi;

    wam::_program_grammar::result_t parser_result;
    wam::parse_program(program_code, parser_result);

    //func -> list of term_codes
    std::unordered_map<wam::functor_view, std::vector<wam::rule>> result;
    result.reserve(program_code.size());

    for(auto& rule : parser_result){
        if(!rule){//If rule is a comment
            continue;
        }
        auto[head_functor, code] = wam::compile_program_term(*rule->children, storage);
        code.set_code_info(rule->code_info);

        auto inserted_func = result.find(head_functor);
        if(inserted_func == result.end()){
            std::vector<wam::rule> func_rules{};
            func_rules.reserve(4);
            func_rules.emplace_back(code);
            result.emplace(std::make_pair(head_functor, func_rules));
        }else{
            inserted_func->second.emplace_back(code);
        }
    }
    return result;
}

std::pair<wam::functor_view, wam::rule> wam::compile_program_term(std::vector<node>& atoms, wam::storage& storage, bool compilation_of_built_in_preds) {
    using namespace std::placeholders;

    const auto permanent_count = assign_permanent_registers(atoms, true);

    ////Generate the term_codes

    wam::rule rule;
    std::vector<std::function<void(wam::executor &)>> instructions;

    //Assign the registers for head + first body combined
    node &head_atom = atoms.at(0);
    if(!compilation_of_built_in_preds){
        compiler::check_and_throw_redefinition_of_built_in_pred(head_atom);
    }
    const wam::functor_view head_atom_functor = head_atom.to_functor_view();
    node *first_body_atom = atoms.size() > 1 ? &atoms[1] : nullptr;
    const int x_a_reg_count = assign_registers(head_atom, first_body_atom);

    //Treat the head as an fact
    const std::vector<const node *> flattened_form = flatten_program(head_atom);
    //The head may have an allocate instruction
    if (permanent_count) {
        instructions.emplace_back(std::bind(wam::allocate, _1, permanent_count));
    }

    std::unordered_map<wam::helper::seen_register, bool> seen_registers;
    to_program_instructions(flattened_form, std::back_inserter(instructions), seen_registers, storage);

    //This is not necessary because if the head is a constant, it has x_reg = 0, and only a var
    //could have a x_reg of 0 if it would be of atom level --> not possible
//    if(head_atom.is_constant()){
//        seen_registers.erase({helper::register_type::X_REG, head_atom.get_x_reg()});
//    }
    //We need to erase seen functor regs, because those shall not interfere with var regs
    if(!head_atom.is_constant()){
        bfs_order(head_atom, true, [&](node* n){
            if(n->is_functor() || n->is_constant()){
                seen_registers.erase({helper::register_type::X_REG, n->get_x_reg()});
            }
        });
    }

    //Build the head
    rule.add_atom(
            x_a_reg_count,
            std::move(instructions),
            //Also find the substitutions in the head atom (for more info see generate_program_instructions at the bottom)
            find_var_reg_substitutions(head_atom),
            head_atom.code_info,
            std::move(head_atom));

    //Clear the generated instructions
    instructions.clear();

    //Let the first body atom compile as query with seen x regs from head
    if (first_body_atom) {
        const std::vector<const node *> first_body_flattened_form = flatten_query(*first_body_atom);
        to_query_instructions(first_body_flattened_form, *first_body_atom, std::back_inserter(instructions),
                              seen_registers,
                              storage);

        rule.add_atom(x_a_reg_count,
                                std::move(instructions),
                                find_var_reg_substitutions(*first_body_atom),
                                std::move(first_body_atom->code_info),
                                std::move(*first_body_atom));
        //Clear the generated instructions
        instructions.clear();
    }

    //Let atoms 2 till end compile as normal querys with seen y regs from before
    if (atoms.size() > 2) {
        //remove x/a regs from before
        remove_x_a_regs(seen_registers);
        for_each(atoms.begin() + 2, atoms.end(), [&](node &atom) {
            compile_query_atom(std::move(atom),
                    seen_registers,
                    instructions,
                    rule,
                    false,
                    storage);
        });
    }

    //All body atoms have been built append a optional deallocate instruction
    if (permanent_count) {
        instructions.emplace_back(std::bind(wam::deallocate, _1));
        rule.add_atom(
                0,
                instructions,
                std::vector<var_reg_substitution>{},
                source_code_info{},
                node{STORED_OBJECT_FLAG::CONSTANT, "DEALLOCATE"});
    }


    return std::make_pair(head_atom_functor, rule);
}


std::vector<const node *> wam::flatten_query(const node &outer_functor) {
    std::vector<const node *> flattened_term = flatten_program(outer_functor);

    /*
     * If the tree would be traversed in postorder with the arguments as roots (skip_root = true) these
     * functions would be unnecessary
     */
    if(outer_functor.is_constant()){
        return flattened_term;
    }
    std::rotate(flattened_term.begin(), flattened_term.begin() + outer_functor.children->size(), flattened_term.end());
    std::reverse(flattened_term.begin(), flattened_term.end() - outer_functor.children->size());
    return flattened_term;
}

//Helper struct for assign_permanent_registers
struct var_information {
    //The atom number of the first occurrence
    int first_occurrence = 0;
    bool is_permanent = false;
    //All nodes containing this var
    std::vector<node *> nodes;
};

/**
 * Assigns permanent registers to nodes which represent a query or program term
 * e.G. f(X), b(H), g(D).
 * or f(X) :- b(H), g(D).
 * @param nodes
 * @param program_term
 * @return the amount of permanent registers
 */
int wam::assign_permanent_registers(std::vector<node>& nodes, bool program_term) {
    //If there is only 1 child or a program term with only head and one body, there is no work to be done
    if (nodes.size() <= 1 || (nodes.size() == 2 && program_term)) {
        return {};
    }
    //Contains a var and the specific information;
    std::unordered_map<std::string, var_information> seen_variables;

    int atom_number = 0;

    if(!nodes.at(0).is_constant()) {
        bfs_order(nodes.at(0), true, [&](node *cur_node) {
            if (cur_node->is_variable()) {
                auto &var_info = seen_variables[cur_node->name];
                var_info.first_occurrence = atom_number;
                var_info.nodes.push_back(cur_node);
            }
        });
    }
    ++atom_number;

    if (program_term && !nodes.at(1).is_constant()) {
        bfs_order(nodes.at(1), true, [&](node *cur_node) {
            if (cur_node->is_variable()) {
                auto &var_info = seen_variables[cur_node->name];
                var_info.first_occurrence = atom_number;
                var_info.nodes.push_back(cur_node);
            }
        });
        ++atom_number;
    }

    for_each(nodes.begin() + 1 + program_term, nodes.end(),
             [&](node &cur_top_node) {
                if(cur_top_node.is_constant()){
                    return;
                }
                 bfs_order(cur_top_node, true, [&](node *cur_node) {
                     if (cur_node->is_variable()) {
                         /*
                          * if this var has been seen, we check if the var has encountered in an earlier atom.
                          * if so, the var is permanent.
                          */
                         if (seen_variables.find(cur_node->name) != seen_variables.end()) {
                             auto &var_information = seen_variables[cur_node->name];
                             var_information.nodes.push_back(cur_node);
                             var_information.is_permanent = var_information.first_occurrence != atom_number;
                         } else {
                             auto &var_information = seen_variables[cur_node->name];
                             var_information.first_occurrence = atom_number;
                             var_information.nodes.push_back(cur_node);
                         }
                     }
                 });
                 ++atom_number;
             });

    size_t y_reg_index = 0;
    for (auto &entry : seen_variables) {
        if (entry.second.is_permanent) {
            for (auto &var_node : entry.second.nodes) {
                var_node->set_y_reg(y_reg_index);
            }
            ++y_reg_index;
        }
    }

    return y_reg_index;
}

std::unordered_map<wam::functor_view, std::vector<wam::rule>> wam::get_build_in_predicates(wam::storage &storage) {
    return compiler::preds::compile_built_in_predicates(storage);
}


