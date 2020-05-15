//
// Created by leonhard on 27.04.20.
//

#ifndef PROLOG_BFS_BUILT_IN_PRED_COMP_H
#define PROLOG_BFS_BUILT_IN_PRED_COMP_H

#include <wam/built_in_predicates/equals.h>
#include <wam/built_in_predicates/not_equals.h>
#include <wam/compiler/util/seen_register.h>
#include <wam/bfs_organizer/data/storage.h>


namespace wam{
    namespace preds{
        struct binary_built_in_pred{
            std::function<void(wam::executor&, size_t, size_t)> func;
            node tree_representation;

            binary_built_in_pred(std::function<void(wam::executor &, size_t, size_t)> func,
                                 node treeRepresentation);
        };


        std::vector<binary_built_in_pred>
        compile_built_in_predicates();
    }
}


#endif //PROLOG_BFS_BUILT_IN_PRED_COMP_H
