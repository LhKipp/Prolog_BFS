//

#include <algorithm>
#include <iterator>
#include "util.h"


void wam::make_to_query(node& top_node, std::vector<node>& atoms){
    wam::make_to_children(top_node, atoms);
    top_node.code_info.line_begin = std::numeric_limits<unsigned>::max();
    top_node.code_info.line_end = std::numeric_limits<unsigned>::max();
    //For now we emit whole query as string, because not needed
}

void wam::make_to_children(node& top_node, std::vector<node>& children){
    top_node.children = std::make_unique<std::vector<node>>(children);
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

void wam::make_to_binary_func(node &result, node &lhs, std::string predicate, node &rhs) {
    make_to_func(predicate, result);
    result.add_to_children(lhs);
    result.add_to_children(rhs);
}

void wam::make_to_sum(node &result, node& product, std::vector<boost::fusion::vector<std::string, node>> &rhs){
    using namespace boost::fusion;
    result.set_type(STORED_OBJECT_FLAG::SUM);
    result.children->push_back(product);
    for(auto add_node : rhs){
        auto math_sign = at_c<0>(add_node) == "+" ? STORED_OBJECT_FLAG::PLUS : STORED_OBJECT_FLAG::MINUS;
        result.children->push_back(node{math_sign});
        result.children->push_back(at_c<1>(add_node));
    }
}

void wam::make_to_product(node &result, node& power, std::vector<boost::fusion::vector<std::string, node>> &rhs){
    using namespace boost::fusion;
    result.set_type(STORED_OBJECT_FLAG::PRODUCT);
    result.children->push_back(power);
    for(auto add_node : rhs){
        auto math_sign = at_c<0>(add_node) == "*" ? STORED_OBJECT_FLAG::MULT : STORED_OBJECT_FLAG::INT_DIV;
        result.children->push_back(node{math_sign});
        result.children->push_back(at_c<1>(add_node));
    }
}

void wam::make_to_power(node &result, node& value, boost::optional<node>& rhs){
    using namespace boost::fusion;
    result.set_type(STORED_OBJECT_FLAG::PRODUCT);
    result.children->push_back(value);
    if(rhs){
        auto math_sign = STORED_OBJECT_FLAG::INT_POW;
        result.children->push_back(node{math_sign});
        result.children->push_back(*rhs);
    }
}

void wam::make_to_int(node &result, std::vector<char> &val) {
    result.set_type(STORED_OBJECT_FLAG::INT);
    result.name = std::string{val.begin(), val.end()};
}
