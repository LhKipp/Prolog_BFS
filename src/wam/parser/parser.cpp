//
// Created by leonhard on 05.07.19.
//

#include <iostream>
#include<stack>
#include <unordered_map>
#include "parser.h"
#include "util/node.h"
#include "util/node_iteration.h"

#include "../data/heap_tag.h"
#include "../instructions/instructions.h"
#include "../data/term_code.h"

node wam::build_tree(const std::string &line) {
    if (!line.size()) {
        std::cout << "In build_tree: empty argument supplied!" << std::endl;
        throw new int();
    }

    node top_node{STORED_OBJECT_FLAG::NONE};
    top_node.children = std::make_unique<std::vector<node>>();

    //If the term is a single variable or constant
    size_t name_end = line.find_first_of('(');
    if (name_end == std::string::npos) {
        if (std::isupper(line[0])) {//If it is a variable
            node var{STORED_OBJECT_FLAG::VARIABLE};
            var.name = line;
            top_node.children->push_back(var);
        } else {//if it is a constant
            node constant{STORED_OBJECT_FLAG::CONSTANT};
            constant.name = line;
            top_node.children->push_back(constant);
        }
        return top_node;
    }

    //If the term has at least one functor


    //holds the current parent node on top.
    std::stack<node *> current_parent;
    //At the begin, its the outer pred
    current_parent.push(&top_node);

    size_t cur_pos = 0;
    while (cur_pos < line.size()) {
        if (line[cur_pos] == ')') {
            current_parent.pop();
            ++cur_pos;
        } else if (line[cur_pos] == ',') {
            ++cur_pos;
        }
            //If the inner term starts Capital it is a variable
        else if (std::isupper(line[cur_pos])) {
            size_t inner_term_end = line.find_first_of(",)", cur_pos);
            size_t name_len = inner_term_end - cur_pos;
            //Assign the inner_term as a child of the current parent
            current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, line.substr(cur_pos, name_len));

            cur_pos += name_len;

        } else {//Inner term is either functor or constant
            auto inner_term_end = line.find_first_of(",)", cur_pos);
            auto opening_brack_pos = line.find_first_of('(', cur_pos);
            //If there is no ( or ,) is before ( the current inner_term is a constant
            if (opening_brack_pos == std::string::npos || inner_term_end < opening_brack_pos) {
                size_t name_len = inner_term_end - cur_pos;
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::CONSTANT,
                                                             line.substr(cur_pos, name_len));

                cur_pos += name_len;
            } else {//inner_term is a functor
                size_t name_len = opening_brack_pos - cur_pos;
                //Assign the functor as a child of the parent and make the functor the current parent
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR,
                                                             line.substr(cur_pos, name_len));
                current_parent.push(&current_parent.top()->children->back());

                //+1 to overread the (
                cur_pos += name_len + 1;
            }
        }
    }

    //The line is now in tree structure
    return top_node;
}

/*
 * Returns a tuple (count of registers, count of functors/constants)
 */
wam::helper::reg_func_counts wam::assign_registers(node &top_node) {
    //variable name --> first occurence of var
    std::unordered_map<std::string, node *> first_occurences;
    size_t regist_index = 0;
    size_t func_view_index = 0;

    bfs_order(top_node, [&](node *node) {
        if (node->type == STORED_OBJECT_FLAG::VARIABLE) {
            if (first_occurences[node->name]) {//if the var has already been seen
                node->regist = first_occurences[node->name]->regist;
            } else {
                node->regist = regist_index;
//                store.emplace_back(heap_tag::REF, regist_index);
                first_occurences[node->name] = node;
                ++regist_index;
            }
        } else {//constant or functor
            //todo possible space saving: make same functors share functor_view cell
            node->regist = regist_index;
            ++regist_index;
            ++func_view_index;
        }
    });

    return wam::helper::reg_func_counts{regist_index, func_view_index};
}

/*
 * returns a flattened form of the term (e.g. for generating instructions).
 * for a program term the first node (sub-term) will be first in vector and so on.
 * for a query term the order will be reversed! the first node will be last in vector!
 */
std::vector<const node *> wam::flatten(const node &top_node) {
    std::vector<const node *> result;

    bfs_order(top_node, [&](const node *node) {
        if (node->type == STORED_OBJECT_FLAG::FUNCTOR || node->type == STORED_OBJECT_FLAG::CONSTANT) {
            result.push_back(node);
        }
    });
    return result;
}


std::vector<std::function<void(wam::executor &)>>
wam::to_query_instructions(const std::vector<const node *> &flattened_term) {
    using namespace std::placeholders;
    std::vector<std::function<void(wam::executor &)>> functions;

    //TODO isnt a set enough?
    std::unordered_map<size_t, bool> seen_registers;

    std::for_each(flattened_term.rbegin(), flattened_term.rend(), [&](const node *node) {
        int children_count = node->children ? node->children->size() : 0;
        functor_view functor_view{node->name, children_count};

        functions.emplace_back(
                std::bind(wam::put_structure, _1, functor_view, node->regist));
        seen_registers[node->regist] = true;

        if(node->is_constant()){//No need to process any children
            return;
        }
        for (auto &childs : *node->children) {
            if (seen_registers[childs.regist]) {
                functions.emplace_back(std::bind(wam::set_value, _1, childs.regist));
            } else {//if not encountered yet
                functions.emplace_back(std::bind(wam::set_variable, _1, childs.regist));
                seen_registers[childs.regist] = true;
            }
        }
    });

    return functions;
}

std::vector<std::function<void(wam::executor &)>>
wam::to_program_instructions(const std::vector<const node *> &flattened_term) {
    using namespace std::placeholders;
    std::vector<std::function<void(wam::executor &)>> functions;

    //TODO isnt a set enough?
    std::unordered_map<size_t, bool> seen_registers;

    std::for_each(flattened_term.begin(), flattened_term.end(), [&](const node *node) {
        int children_count = node->children ? node->children->size() : 0;
        functor_view functor_view{node->name, children_count};

        functions.emplace_back(
                std::bind(wam::get_structure, _1, functor_view, node->regist));
        seen_registers[node->regist] = true;

        if (node->is_constant()) {//no need to process children if node is constant
            return;
        }
        //If node is functor
        for (auto &childs : *node->children) {
            if (seen_registers[childs.regist]) {
                functions.emplace_back(std::bind(wam::unify_value, _1, childs.regist));
            } else {//if not encountered yet
                functions.emplace_back(std::bind(wam::unify_variable, _1, childs.regist));
                seen_registers[childs.regist] = true;
            }
        }
    });

    return functions;
}


std::tuple<wam::term_code, std::vector<wam::var_reg_substitution>> wam::parse_query(const std::string &line) {
    //todo add some logic to remove ?- and . ???

    node top_node = wam::build_tree(line);

    wam::helper::reg_func_counts counts = assign_registers(top_node);

    std::vector<var_reg_substitution> substitutions = find_substitutions(top_node);

    std::vector<const node *> flattened_form = flatten(top_node);

    return std::make_tuple(wam::term_code{counts,
                                          make_functor_view(top_node.children->operator[](0)),
                                          to_query_instructions(flattened_form)},
                           substitutions);
}

wam::term_code wam::parse_program_term(const std::string &line) {
    //todo add some logic to remove . ???
    node top_node = build_tree(line);
    wam::helper::reg_func_counts counts = assign_registers(top_node);
    std::vector<const node *> flattened_form = flatten(top_node);

    return wam::term_code{counts,
                          make_functor_view(top_node.children->operator[](0)),
                          to_program_instructions(flattened_form)};
}

functor_view wam::make_functor_view(const node &node) {
    int arity = node.children ? node.children->size() : 0;
    return functor_view{node.name, arity};
}

/*
 * Finds and lists the variable name to register substitutions
 */
std::vector<wam::var_reg_substitution> wam::find_substitutions(const node &top_node) {
    std::vector<wam::var_reg_substitution>result;

    bfs_order(top_node, [&](const node *cur_node) {
        if (cur_node->type == STORED_OBJECT_FLAG::VARIABLE) {
            result.emplace_back(cur_node->name, cur_node->regist);
        }
    });

    return result;
}
















































