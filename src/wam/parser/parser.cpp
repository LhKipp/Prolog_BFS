//
// Created by leonhard on 05.07.19.
//

#include <iostream>
#include<stack>
#include <unordered_map>
#include <cassert>
#include "parser.h"
#include "util/node.h"
#include "util/node_iteration.h"

#include "../data/heap_tag.h"
#include "../instructions/instructions.h"
#include "../data/term_code.h"

node wam::build_tree(const std::string &line) {
    if (line.empty()) {
        std::cout << "In build_tree: empty argument supplied!" << std::endl;
        return node{};
    }

    node top_node{STORED_OBJECT_FLAG::NONE};
    top_node.children = std::make_unique<std::vector<node>>();

    //holds the current parent node on top.
    std::stack<node *> current_parent;
    //At the begin, its the outer pred
    current_parent.push(&top_node);

    size_t cur_pos = 0;
    while (cur_pos < line.size()) {
        if (line[cur_pos] == '.') {
            break;
        } else if (line[cur_pos] == ')') {
            current_parent.pop();
            ++cur_pos;
        }
            //If the inner term starts Capital it is a variable
        else if (std::isupper(line[cur_pos])) {
            const size_t inner_term_end = line.find_first_of(",).", cur_pos);
            const size_t name_len = inner_term_end - cur_pos;
            //Assign the inner_term as a child of the current parent
            current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, line.substr(cur_pos, name_len));

            cur_pos += name_len;

        } else if (std::islower(line[cur_pos])) {//Inner term is either functor or constant
            const auto inner_term_end = line.find_first_of(",).", cur_pos);
            const auto opening_brack_pos = line.find_first_of('(', cur_pos);
            //If there is no ( or ,) is before ( the current inner_term is a constant
            if (opening_brack_pos == std::string::npos || inner_term_end < opening_brack_pos) {
                const size_t name_len = inner_term_end - cur_pos;
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::CONSTANT,
                                                             line.substr(cur_pos, name_len));

                cur_pos += name_len;
            } else {//inner_term is a functor
                const size_t name_len = opening_brack_pos - cur_pos;
                //Assign the functor as a child of the parent and make the functor the current parent
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR,
                                                             line.substr(cur_pos, name_len));
                current_parent.push(&current_parent.top()->children->back());

                //+1 to overread the (
                cur_pos += name_len + 1;
            }
        } else {//sign may be " " , \n
            ++cur_pos;
        }
    }

    //The line is now in tree structure
    return top_node;
}

/*
 * Assigns register to an functor (constant is also viable)
 * Returns a tuple (count of registers, count of functors/constants)
 */
wam::helper::reg_func_counts wam::assign_registers(node &functor) {
    if (functor.is_constant()) {
        functor.set_x_reg(0);
        functor.set_a_reg(0);
        return {1, 1};
    }
    //functor is a normal functor

    //first we assign the arguments a_registers
    size_t reg_index = 0;
    size_t func_counter = 0;
    for (auto &node : *functor.children) {
        node.set_a_reg(reg_index);
        node.set_x_reg(reg_index);

        func_counter += !node.is_variable();
        ++reg_index;
    }

    //Now we assign the x_registers

    //variable name --> first occurence of var
    std::unordered_map<std::string, node *> first_occurences;

    auto assign_var_x_reg = [&](node *node) {
        if (first_occurences[node->name]) {//if the var has already been seen
            node->set_x_reg(first_occurences[node->name]->get_x_reg());
        } else {
            node->set_x_reg(reg_index);
            first_occurences[node->name] = node;
            ++reg_index;
        }
    };

    //skip functor = true
    //functor is outer node and does not need an register
    bfs_order(functor, true, [&](node *node) {
        //If The node is an argument it has an a_reg
        //Only arguments that are vars will also be assigned an x_reg
        if (node->is_argument()) {
            if (node->is_variable()) {
                assign_var_x_reg(node);
                return;
            } else {
                //The node is an constant or functor and has an a_reg
                return;
            }
        }

        //The node is not an argument
        if (node->is_variable()) {
            assign_var_x_reg(node);
        } else {//constant or functor
            node->set_x_reg(reg_index);
            ++reg_index;
            ++func_counter;
        }
    });

    return wam::helper::reg_func_counts{reg_index, func_counter};
}

/*
 * returns a flattened form of the term (e.g. for generating instructions).
 * for a query term the order will be reversed! the first node will be last in vector!
 *
 * Note: The outer functor will be the first element in the returned vector
 */
std::vector<const node *> wam::flatten(const node &outer_functor) {
    std::vector<const node *> result;

    bfs_order(outer_functor, true, [&](const node *node) {
        if (node->is_functor()
            || node->is_constant()
            || node->is_argument()) {
            result.push_back(node);
        }
    });
    return result;
}


std::vector<std::function<void(wam::executor &)>>
wam::to_query_instructions(const std::vector<const node *> &flattened_term, const node &outer_functor) {
    using namespace std::placeholders;
    std::vector<std::function<void(wam::executor &)>> functions;

    //TODO isnt a set enough?
    std::unordered_map<size_t, bool> seen_registers;

    std::for_each(flattened_term.begin(), flattened_term.end(), [&](const node *node) {
        if (node->is_variable()) {//Every node in flattened term, beeing an variable is an argument!
            if (seen_registers[node->get_x_reg()]) {//If the var has been seen
                if(node->is_permanent()){
                    functions.emplace_back(std::bind(wam::put_permanent_value, _1, node->get_y_reg(), node->get_a_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::put_value, _1, node->get_x_reg(), node->get_a_reg()));
                }
            } else { //If the var hasn't been seen
                seen_registers[node->get_x_reg()] = true;
                if(node->is_permanent()){
                    functions.emplace_back(std::bind(wam::put_permanent_variable, _1, node->get_y_reg(), node->get_a_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::put_variable, _1, node->get_x_reg(), node->get_a_reg()));
                }
            }
            return;
        }
        functor_view functor_view = node->to_functor_view();

        functions.emplace_back(
                std::bind(wam::put_structure, _1, functor_view, node->get_x_reg()));
        seen_registers[node->get_x_reg()] = true;

        if (node->is_constant()) {//No need to process any children
            return;
        }
        for (auto &childs : *node->children) {
            if (seen_registers[childs.get_x_reg()]) {
                if(node->is_permanent()){//Node
                    functions.emplace_back(std::bind(wam::set_permanent_value, _1, childs.get_y_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::set_value, _1, childs.get_x_reg()));
                }
            } else {//if not encountered yet
                if(node->is_permanent()){
                    functions.emplace_back(std::bind(wam::set_permanent_variable, _1, childs.get_y_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::set_variable, _1, childs.get_x_reg()));
                }
                seen_registers[childs.get_x_reg()] = true;
            }
        }
    });

    functions.emplace_back(std::bind(wam::call, _1, outer_functor.to_functor_view()));

    return functions;
}

std::vector<std::function<void(wam::executor &)>>
wam::to_program_instructions(const std::vector<const node *> &flattened_term) {
    using namespace std::placeholders;
    std::vector<std::function<void(wam::executor &)>> functions;

    //TODO isnt a set enough?
    std::unordered_map<size_t, bool> seen_registers;

    //The first element is the outer functor
    std::for_each(flattened_term.begin(), flattened_term.end(), [&](const node *node) {

        if (node->is_variable()) {//If the node is an variable argument
            if (seen_registers[node->get_x_reg()]) {//If the var has been seen
                if(node->is_permanent()){
                    functions.emplace_back(std::bind(wam::get_permanent_value, _1, node->get_y_reg(), node->get_a_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::get_value, _1, node->get_x_reg(), node->get_a_reg()));
                }
            } else { //If the var hasn't been seen
                //TODO Test if it works without
//                seen_registers[node->get_a_reg()] = true;
                seen_registers[node->get_x_reg()] = true;
                if(node->is_permanent()){
                    functions.emplace_back(std::bind(wam::get_permanent_variable, _1, node->get_y_reg(), node->get_a_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::get_variable, _1, node->get_x_reg(), node->get_a_reg()));
                }
            }
            //It is a var, no need to process children
            return;
        }

        functor_view functor_view = node->to_functor_view();
        functions.emplace_back(
                std::bind(wam::get_structure, _1, functor_view, node->get_x_reg()));
        seen_registers[node->get_x_reg()] = true;

        if (node->is_constant()) {//no need to process children if node is constant
            return;
        }
        //If node is functor
        for (auto &childs : *node->children) {
            if (seen_registers[childs.get_x_reg()]) {
                if(childs.is_permanent()){
                    functions.emplace_back(std::bind(wam::unify_permanent_value, _1, childs.get_y_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::unify_value, _1, childs.get_x_reg()));
                }
            } else {//if not encountered yet
                if(childs.is_permanent()){
                    functions.emplace_back(std::bind(wam::unify_permanent_variable, _1, childs.get_y_reg()));
                }else{
                    functions.emplace_back(std::bind(wam::unify_variable, _1, childs.get_x_reg()));
                }
                seen_registers[childs.get_x_reg()] = true;
            }
        }
    });

    functions.emplace_back(std::bind(wam::proceed, _1));

    return functions;
}


std::tuple<std::vector<wam::term_code>, std::vector<wam::var_reg_substitution>>
wam::parse_query(const std::string &line) {
    //todo add some logic to remove ?- and . ???
    node program_node = wam::build_tree(line);

    //The instructions for each head.
    std::vector<term_code> instructions;
    const size_t permanent_count = assign_permanent_registers(program_node, false);

    for (auto &outer_functor : *program_node.children) {
        wam::helper::reg_func_counts counts = assign_registers(outer_functor);
        counts.y_regs_counts = permanent_count;

        std::vector<const node *> flattened_form = flatten_query(outer_functor);

        instructions.emplace_back(counts,
                                  outer_functor.to_functor_view(),
                                  to_query_instructions(flattened_form, outer_functor));
    }

    std::vector<var_reg_substitution> substitutions = find_substitutions(program_node);
    return std::make_tuple(instructions, substitutions);

}

std::vector<wam::term_code> wam::parse_program_term(const std::string &line) {
    //todo add some logic to remove . ???
    node program_node = build_tree(line);

    const size_t permanent_count = assign_permanent_registers(program_node, true);
    std::vector<node> & atoms = *program_node.children;

    std::vector<term_code> instructions;

    //Treat the head as an fact
    node &outer_functor = atoms.at(0);
    wam::helper::reg_func_counts counts = assign_registers(outer_functor);
    counts.y_regs_counts= permanent_count;
    std::vector<const node *> flattened_form = flatten(outer_functor);
    instructions.emplace_back(counts,
                                outer_functor.to_functor_view(),
                                to_program_instructions(flattened_form));

//    if(program_node.children->size() == 1){
//        //it is a simple fact, translation is finished
//        return instructions;
//    }
//    The term is a rule

    for_each(std::next(atoms.begin()), atoms.end(),[&](node& atom){
        wam::helper::reg_func_counts counts = assign_registers(atom);
        std::vector<const node *> flattened_form = flatten(atom);
        instructions.emplace_back(counts,
                                outer_functor.to_functor_view(),
                                to_program_instructions(flattened_form));
    });

    return instructions;
}

/*
 * Finds and lists the variable name to register substitutions
 */
std::vector<wam::var_reg_substitution> wam::find_substitutions(const node &top_node) {
    std::vector<wam::var_reg_substitution> result;

    bfs_order(top_node, true, [&](const node *cur_node) {
        if (cur_node->is_variable()) {
            //TODO is x_reg enough?
            if(cur_node->is_permanent()){
                result.emplace_back(cur_node->name, cur_node->get_y_reg(), true);
            }else{
                result.emplace_back(cur_node->name, cur_node->get_x_reg(), false);
            }
        }
    });

    return result;
}

std::vector<const node *> wam::flatten_query(const node &outer_functor) {
    std::vector<const node *> flattened_term = flatten(outer_functor);

    /*
     * If the tree would be traversed in postorder with the arguments as roots (skip_root = true) these
     * functions would be unnecessary
     */
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

size_t wam::assign_permanent_registers(const node &program_node, bool program_term) {
    //If there is only 1 child or a program term with only head and one body, there is no work to be done
    if (program_node.children->size() <= 1 || (program_node.children->size() == 2 && program_term)) {
        return 0;
    }
    //Contains a var and the specific information;
    std::unordered_map<std::string, var_information> seen_variables;

    int atom_number = 0;

    bfs_order(program_node.children->at(0), true, [&](node *cur_node) {
        if (cur_node->is_variable()) {
            seen_variables[cur_node->name].first_occurrence = atom_number;
        }
    });

    if (program_term) {
        bfs_order(program_node.children->at(1), true, [&](node *cur_node) {
            if (cur_node->is_variable()) {
                seen_variables[cur_node->name].first_occurrence = atom_number;
            }
        });
    }

    for_each(program_node.children->begin() + 1 + program_term, program_node.children->end(),
             [&](node &cur_top_node) {
                 ++atom_number;
                 bfs_order(program_node.children->at(1), true, [&](node *cur_node) {
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
















































