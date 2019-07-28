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
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::CONSTANT, line.substr(cur_pos, name_len));

                cur_pos += name_len;
            } else {//inner_term is a functor
                size_t name_len = opening_brack_pos - cur_pos;
                //Assign the functor as a child of the parent and make the functor the current parent
                current_parent.top()->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR, line.substr(cur_pos, name_len));
                current_parent.push(&current_parent.top()->children->back());

                //+1 to overread the (
                cur_pos += name_len + 1;
            }
        }
    }

    //The line is now in tree structure
    return top_node;
}

void wam::assign_registers(wam::executor &executor, node &top_node) {
    //variable name --> first occurence of var
    std::unordered_map<std::string, node *> first_occurences;
    int regist_index = executor.registers.size();
    int func_view_index = executor.functors.size();

    bfs_order(top_node, [&](node *node) {
        if (node->type == STORED_OBJECT_FLAG::VARIABLE) {
            if (first_occurences[node->name]) {//if the var has already been seen
                node->regist = first_occurences[node->name]->regist;
            } else {
                node->regist = regist_index;
                executor.registers.emplace_back(heap_tag::REF, regist_index);
                first_occurences[node->name] = node;
                ++regist_index;
            }
        } else {//constant or functor
            node->regist = regist_index;
            executor.registers.emplace_back(heap_tag::FUN, func_view_index);
            //For constants, children == nullptr
            auto children_count = node->children ? node->children->size() : 0;
            executor.functors.emplace_back(node->name, children_count);
            ++regist_index;
            ++func_view_index;
        }
    });

}

/*
 * Returns a flattened query in REVERSED order!.
 */
std::vector<const node *> wam::flatten_query(wam::executor &executor, const node &top_node) {
    std::vector<const node *> result;

    bfs_order(top_node, [&](const node * node){
        if(node->type == STORED_OBJECT_FLAG::FUNCTOR || node->type == STORED_OBJECT_FLAG::CONSTANT){
            result.push_back(node);
        }
    });
    return result;
}

void wam::parse_query(wam::executor &executor, const std::string &line) {
    //TODO Add some logic to remove ?- and . ???
    node top_node = build_tree(line);
    assign_registers(executor, top_node);
    std::vector<const node *> flattened_form = flatten_query(executor, top_node);


}












































