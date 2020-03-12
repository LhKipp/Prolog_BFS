//
// Created by leonhard on 11.03.20.
//

#include "../../wam/visual/unification_tree/query_node.h"
#include <emscripten/bind.h>
//#include "var_binding_node.cpp"
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(query_node) {

    register_vector<wam::var_binding_node>("vector<var_binding_node>");
//        class_<wam::var_binding_node>("VarBindingNode");
        class_<wam::query_node>("QueryNode")
                .function("failed", &wam::query_node::failed)
//                .function("get_children", &wam::query_node::get_children)
                .function("get_query_as_str", &wam::query_node::get_query_as_str)
                .function("get_query_code_line", &wam::query_node::get_query_code_line)

        ;

}
