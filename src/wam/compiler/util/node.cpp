//
// Created by leonhard on 22.03.20.
//


#include "node.h"
#include <string>
#include <algorithm>
#include <wam/compiler/parser/parsed_helper_types/binary_arithmetic_predicate.h>
#include <wam/compiler/parser/parsed_helper_types/expressions.h>
#include <wam/compiler/parser/parsed_helper_types/prolog_data_types.h>

std::string node::to_string() const {
    if(is_variable() || is_constant() || is_int()) return name;
    if(is_empty_list()) return "[]";
    if(is_list()){//its a list
        if(children->size() == 1){
            assert(false);
            return "[" + children->operator[](0).to_string() + "]";
        }

        //More than 1 child
        std::string start;
        start.reserve(32);
        start = "[" + children->operator[](0).to_string();

        const node* next_list = &children->back();
        while(true){
            if(!next_list->is_list()) {
                return start + ", " + next_list->to_string() + "]";
            }
            if(next_list->is_append_functor()) {
                assert(false);
                return start + " |" + next_list->children->operator[](0).to_string() + " ]";
            }
            if(next_list->children->size() == 1) {
                assert(false);
                return start + ", " + next_list->children->operator[](0).to_string() + " ]";
            }
            if(next_list->is_empty_list()){
                return start + "]";
            }

            start += ", " + next_list->children->operator[](0).to_string();
            next_list = &next_list->children->back();
        }
    }

    if(is_functor()){
        std::string start;
        start.reserve(name.size() + name.size() * children->size());

        start += name + "(";
        std::for_each(children->begin(), children->end() -1, [&](const auto& child){
            start += child.to_string() + ", ";
        });
        start += children->back().to_string() + ")";
        return start;
    }

    //NONE
    return "";
}


node::node(const parser::binary_arithmetic_predicate& p): type{STORED_OBJECT_FLAG::FUNCTOR}, children{std::make_unique<std::vector<node>>()}{
    name = p.pred;
    children->push_back(p.lhs);
    children->push_back(p.rhs);
}

node::node(const parser::chained_expr &p): type{p.type}, children{std::make_unique<std::vector<node>>()}{
    children->push_back(p.value);
    for(auto &chained_value : p.chained_values){
        children->emplace_back(chained_value.math_op);
        children->push_back(chained_value.val);
    }
}

node::node(const parser::opt_chained_value &p): type{p.type}, children{std::make_unique<std::vector<node>>()}{
    children->push_back(p.value);
    if(p.chained_val){
        children->push_back(node{p.chained_val->math_op});
        children->push_back(p.chained_val->val);
    }
}

node::node(const parser::functor &f): type{STORED_OBJECT_FLAG::FUNCTOR}, name{f.name},
                                      children{std::make_unique<std::vector<node>>(f.children)} {
}

node::node(const parser::normal_list &l): type{STORED_OBJECT_FLAG::FUNCTOR}, name{"["},
                                      children{std::make_unique<std::vector<node>>()} {
    if(l.children.empty()){
        return;
    }
    //Now transform ',' seperated list to list(x, list(y, list(...
    node* cur_parent = this;
    //if list is of type [a,b,c,d] (no finish through "|")
    std::for_each(l.children.begin(), l.children.end(), [&](auto &child) {
        cur_parent->children->push_back(std::move(child));
        cur_parent->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR, "[");
        cur_parent = &cur_parent->children->back();
    });
}

node::node(const parser::finished_list &l): type{STORED_OBJECT_FLAG::FUNCTOR}, name{"["},
                                          children{std::make_unique<std::vector<node>>()} {
    if(l.children.empty()){
        return;
    }
    //Now transform ',' seperated list to list(x, list(y, list(...
    node* cur_parent = this;
    //if list is of type [a,b,c,d | e] (finish through "|")
    std::for_each(l.children.begin(), l.children.end() -1, [&](auto &child) {
        cur_parent->children->push_back(std::move(child));
        cur_parent->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR, "[");
        cur_parent = &cur_parent->children->back();
    });
    cur_parent->children->push_back(*(l.children.end() - 1));
    cur_parent->children->push_back(l.end);
}

