//
// Created by leonhard on 22.03.20.
//


#include "node.h"
#include <string>
#include <algorithm>

std::string node::to_string() const {
    if(is_variable() || is_constant()) return name;
    if(is_empty_list()) return "[]";
    if(is_functor()){
        std::string start;
        start.reserve(name.size() + name.size() * children->size());

        start += name + "(";
        std::for_each(children->begin(), children->end() -1, [&](const auto& child){
             start += child.to_string() + ", ";
        });
        start += children->back().to_string() + ")";
        return start;
    }else if(is_list()){//its a list
        if(children->size() == 1){
            return "[" + children->operator[](0).to_string() + "]";
        }
        //More than 1 child
        std::string start;
        start.reserve(children->size() * 5);

        start += "[" + children->operator[](0).to_string();
        //If child->size == 2, begin+1 == end-1
        std::for_each(children->begin() +1, children->end() -1, [&](const auto& child){
            start += ", " + child.to_string();
        });
        if(children->back().is_append_functor()){
            start += " |" + children->back().to_string();
        }else{//normal end
            start += ", " + children->back().to_string();
        }

        return start;
    }else{
        return "";
    }
}

