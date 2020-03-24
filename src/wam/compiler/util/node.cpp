//
// Created by leonhard on 22.03.20.
//


#include "node.h"
#include <string>
#include <algorithm>

std::string node::to_string() const {
    if(is_variable() || is_constant()) return name;
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

