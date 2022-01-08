//
// Created by leonhard on 07.03.20.
//

#include "prolog_bfs/wam/visual/unification_tree/query_node.h"
#include "prolog_bfs/wam/visual/unification_tree/var_binding_node.h"



wam::var_binding_node::var_binding_node(const wam::var_binding_node &other) {
    first_fact_binding = other.first_fact_binding;
    var_bindings = other.var_bindings;
    exec = other.exec;
    _id = other._id;
    if(other.continues()){
        child = std::make_unique<query_node>(other.get_continuing_query());
    }else if(other.succeeded()){
        child = other.get_final_var_bindings();
    }
}

wam::var_binding_node &wam::var_binding_node::operator=(const wam::var_binding_node &other) {
    first_fact_binding = other.first_fact_binding;
    var_bindings = other.var_bindings;
    _id = other._id;
    exec = other.exec;
    if(other.continues()){
        child = std::make_unique<query_node>(other.get_continuing_query());
    }else if(other.succeeded()){
        child = other.get_final_var_bindings();
    }
    return *this;
}

const wam::query_node &wam::var_binding_node::get_continuing_query() const {
    assert(continues());
    return *std::get<std::unique_ptr<query_node>>(child);
}

wam::query_node &wam::var_binding_node::get_continuing_query() {
    assert(continues());
    return *std::get<std::unique_ptr<query_node>>(child);
}

const wam::query_node &wam::var_binding_node::get_continuing_query_wasm() const {
    return get_continuing_query();
}

void wam::var_binding_node::set_continuing_query(wam::query_node q_node) {
    child = std::make_unique<wam::query_node>(std::move(q_node));
}

