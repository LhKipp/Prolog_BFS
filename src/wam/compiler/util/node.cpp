//
// Created by leonhard on 22.03.20.
//


#include "prolog_bfs/wam/compiler/util/node.h"
#include <string>
#include <algorithm>
#include <prolog_bfs/wam/compiler/parser/parsed_helper_types/binary_arithmetic_predicate.h>
#include <prolog_bfs/wam/compiler/parser/parsed_helper_types/expressions.h>
#include <prolog_bfs/wam/compiler/parser/parsed_helper_types/prolog_data_types.h>

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
    if(is_evaluable_functor()){//
        switch(children->size()){
            //TODO output without nodes without outer brackets if not needed
            //E.g. (1 + 1) + 1 -> 1 + 1 + 1
            //But  (1 + 1) * 2 -> (1 + 1) * 2 as brackets have meaning
            case 1:{
                return '(' + name + '(' + children->at(0).to_string() + ')' + ')';
            }
            case 2:
                return '(' + children->at(0).to_string() + ' ' +  name + ' ' + children->at(1).to_string() + ')';
            default:
                //Never happens
                assert(false);
        }
    }

    //NONE
    return "";
}

node::node(const parser::binary_arithmetic_predicate& p): type{STORED_OBJECT_FLAG::FUNCTOR}, children{std::make_unique<std::vector<node>>()}{
    name = p.pred;
    children->push_back(p.lhs);
    children->push_back(p.rhs);
}

node::node(const parser::chained_expr &p){
    //If it is a simple value
    if(p.chained_values.empty()){
        *this = std::move(p.value);
        return;
    }

    node cur_parent{STORED_OBJECT_FLAG ::EVALUABLE_FUNCTOR, p.chained_values.back().math_op};
    cur_parent.children->resize(2);
    cur_parent.children->at(1) = p.chained_values.back().val;
    std::for_each(p.chained_values.rbegin() + 1, p.chained_values.rend(), [&](auto& chained_val){
        cur_parent.children->at(0) = std::move(chained_val.val);
        node before{STORED_OBJECT_FLAG ::EVALUABLE_FUNCTOR, chained_val.math_op};
        before.children->resize(2);
        before.children->at(1) = std::move(cur_parent);
        cur_parent = before;
    });
    cur_parent.children->at(0) = p.value;

    *this = std::move(cur_parent);
}

node::node(const parser::opt_chained_value &p){
    if(!p.chained_val){
        *this = p.value;
        return;
    }

    //Its a real expr
    this->type = STORED_OBJECT_FLAG::EVALUABLE_FUNCTOR;
    this->name = p.chained_val->math_op;
    this->children = std::make_unique<std::vector<node>>(2);
    children->push_back(p.value);
    children->push_back(p.chained_val->val);
    this->code_info = code_info_for_built_in("Lhs" + this->name + "Rhs");
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
