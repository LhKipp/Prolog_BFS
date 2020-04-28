//
// Created by leonhard on 27.04.20.
//

#ifndef PROLOG_BFS_BUILT_IN_PRED_COMP_H
#define PROLOG_BFS_BUILT_IN_PRED_COMP_H

#include <wam/compiler/built_in_predicates/predicates/equals.h>
#include <wam/compiler/built_in_predicates/predicates/not_equals.h>


namespace wam{
    namespace preds{
        struct binary_built_in_pred{
            std::function<void(wam::executor&, size_t, size_t)> func;
            node tree_representation;

            binary_built_in_pred(std::function<void(wam::executor &, size_t, size_t)> func,
                                 node treeRepresentation);
        };


        std::vector<binary_built_in_pred>
        get_binary_built_in_preds();
    }
}


#endif //PROLOG_BFS_BUILT_IN_PRED_COMP_H
