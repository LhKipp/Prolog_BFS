//
// Created by leonhard on 03.03.20.
//

#include <algorithm>
#include "util.h"

template<typename fusion_vec>
std::string wam::boost_vec_to_string(fusion_vec vec) {
    using boost::fusion::at_c;
    std::string result;
    result += at_c<0>(vec);
    std::copy(std::begin(at_c<1>(vec)), std::end(at_c<1>(vec)),
              std::back_insert_iterator(result));
    return result;
}

void wam::make_to_top_node(boost::spirit::unused_type unused, node &n) {
    n.set_type(STORED_OBJECT_FLAG::NONE);
    n.children = std::make_unique<std::vector<node>>();
}

void wam::make_to_cons(std::string &name, node &node) {
    node.set_type(STORED_OBJECT_FLAG::CONSTANT);
    node.name = name;
}

void wam::make_to_func(std::string_view name, node &n) {
    n.set_type(STORED_OBJECT_FLAG::FUNCTOR);
    n.name = name;
    n.children = std::make_unique<std::vector<node>>();
}

void wam::make_to_var(std::string_view name, node &n) {
    n.set_type(STORED_OBJECT_FLAG::VARIABLE);
    n.name = name;
}

void wam::make_to_list(char name, node &n) {
    //For now every list name must be: "["
    assert(name == '[');
    n.set_type(STORED_OBJECT_FLAG::FUNCTOR);
    n.name = name;
    n.children = std::make_unique<std::vector<node>>();
}

void wam::set_list_finished(char unused_attribute, node &node) {
    node.is_finished_list = true;
}

void wam::childs_to_list(node &list_start, char unused_attribute) {
    if (list_start.children->empty()) {
        return;
    }

    //New childs
    auto new_childs = std::make_unique<std::vector<node>>();
    list_start.children.swap(new_childs);
    //New childs now contains old childs (the ',' seperated list of prolog elements


    //Now transform ',' seperated list to list(x, list(y, list(...
    node *cur_parent = &list_start;
    if (!list_start.is_finished_list) {
        //if list is of type [a,b,c,d] (no finish through "|")
        std::for_each(new_childs->begin(), new_childs->end(), [&](auto &child) {
            cur_parent->children->push_back(std::move(child));
            cur_parent->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR, "[");
            cur_parent = &cur_parent->children->back();
        });
    } else {
        //if list is of type [a,b,c,d | e] (finish through "|")
        std::for_each(new_childs->begin(), new_childs->end() - 2, [&](auto &child) {
            cur_parent->children->push_back(std::move(child));
            cur_parent->children->emplace_back(STORED_OBJECT_FLAG::FUNCTOR, "[");
            cur_parent = &cur_parent->children->back();
        });
        cur_parent->children->push_back(std::move(*(new_childs->end() - 2)));
        cur_parent->children->push_back(std::move(*(new_childs->end() - 1)));
    }
}
