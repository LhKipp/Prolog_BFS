//
// Created by leonhard on 07.03.20.
//

#include "query_node.h"
#include "var_binding_node.h"


wam::var_binding_node::var_binding_node(const wam::term_code *calledFunctor, std::vector<wam::var_binding> varBindings,
                                        std::vector<wam::var_binding> final_orig_var_bindings):
                                        state{EXEC_STATE::SUCCESS},
                                        called_functor(calledFunctor),
                                        var_bindings(std::move(varBindings)),
                                        child(std::move(final_orig_var_bindings)){

}

wam::var_binding_node::var_binding_node(const wam::term_code *calledFunctor, std::vector<wam::var_binding> varBindings,
                                        wam::query_node following_query)
                                        :state{EXEC_STATE::ARCHIVED},
                                        called_functor(calledFunctor),
                                        var_bindings(std::move(varBindings)),
                                        child(std::make_unique<wam::query_node>(std::move(following_query)))
                                        {

}

wam::var_binding_node::var_binding_node(wam::term_code *term_code,
        std::vector<var_binding> intermediate_bindings) :
        state{EXEC_STATE::RUNNING},
        called_functor{term_code},
        var_bindings{std::move(intermediate_bindings)}
        {
}

wam::var_binding_node::var_binding_node(const wam::var_binding_node &other) {
    state = other.state;
    called_functor = other.called_functor;
    var_bindings = other.var_bindings;
    if(other.continues()){
        child = std::make_unique<query_node>(other.get_continuing_query());
    }else if(other.succeeded()){
        child = other.get_final_var_bindings();
    }
}

wam::var_binding_node &wam::var_binding_node::operator=(const wam::var_binding_node &other) {
    state = other.state;
    called_functor = other.called_functor;
    var_bindings = other.var_bindings;
    if(other.continues()){
        child = std::make_unique<query_node>(other.get_continuing_query());
    }else if(other.succeeded()){
        child = other.get_final_var_bindings();
    }
    return *this;
}

wam::var_binding_node::~var_binding_node() {
    if(continues()){
        delete std::get<std::unique_ptr<query_node>>(child).release();
    }
    child = std::monostate{};
}

const wam::query_node &wam::var_binding_node::get_continuing_query() const {
    assert(continues());
    return *std::get<std::unique_ptr<query_node>>(child);
}

wam::query_node *wam::var_binding_node::get_continuing_query() {
    assert(continues());
    return std::get<std::unique_ptr<query_node>>(child).get();
}
