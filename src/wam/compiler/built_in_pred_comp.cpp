//
// Created by leonhard on 27.04.20.
//

#include "built_in_pred_comp.h"

#include <utility>
#include <wam/compiler/built_in_predicates/predicates/arithmetic/is.h>

using namespace wam::preds;
std::vector<binary_built_in_pred> wam::preds::compile_built_in_predicates() {
    std::vector<binary_built_in_pred> result;
    result.emplace_back(&equals_check, equals_node_tree());
    result.emplace_back(&not_equals_check, not_equals_node_tree());

    //Arithmetik
    result.emplace_back(&is, is_node_tree());
    return result;
}


binary_built_in_pred::binary_built_in_pred(std::function<void(wam::executor &, size_t, size_t)> func,
                                           node treeRepresentation) : func(std::move(func)),
                                                                             tree_representation(std::move(treeRepresentation)) {}
