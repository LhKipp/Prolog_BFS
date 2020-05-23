//
// Created by leonhard on 11.03.20.
//

#include "../../wam/visual/unification_tree/query_node.h"
#include <emscripten/bind.h>
//#include "var_binding_node.cpp"
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(query_node) {

    //class_<wam::var_binding_node>("VarBindingNode");
    class_<wam::query_node>("QueryNode")
            .function("getNodeID", &wam::query_node::get_node_id)
            .function("isToBeContinued", &wam::query_node::is_to_be_continued)
            .function("isOOMNode", &wam::query_node::is_oom_node)
            .function("failed", &wam::query_node::failed)
            .function("getChildren", &wam::query_node::get_children_wasm)
            .function("getQueryAsString", &wam::query_node::get_query_as_str)
            .function("getQueryCodeLine", &wam::query_node::get_query_code_line)

    ;
    register_vector<wam::var_binding_node>("vector<var_binding_node>");

}
