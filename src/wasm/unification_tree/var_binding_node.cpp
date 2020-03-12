//
// Created by leonhard on 11.03.20.
//

#include "../../wam/visual/unification_tree/var_binding_node.h"
#include "../../wam/visual/unification_tree/query_node.h"

#include <emscripten/bind.h>
using namespace emscripten;

// Binding code
EMSCRIPTEN_BINDINGS(var_binding_node) {
        class_<wam::query_node>("QueryNode");
        class_<wam::var_binding_node>("VarBindingNode")
                .constructor()
                .function("get_var_bindings_as_str", &wam::var_binding_node::get_var_bindings_as_str)
                .function("failed", &wam::var_binding_node::failed)
                .function("is_to_be_continued", &wam::var_binding_node::is_to_be_continued)
                .function("succeeded", &wam::var_binding_node::succeeded)
                .function("get_final_var_bindings_as_str", &wam::var_binding_node::get_final_var_bindings_as_str)
                .function("continues", &wam::var_binding_node::continues)
                .function("get_continuing_query", &wam::var_binding_node::get_continuing_query)
                .function("get_fact_as_str", &wam::var_binding_node::get_fact_as_str)
                .function("get_fact_code_line", &wam::var_binding_node::get_fact_code_line)
        ;
}
