//
// Created by leonhard on 27.04.20.
//

#ifndef PROLOG_BFS_BUILT_IN_PRED_COMP_H
#define PROLOG_BFS_BUILT_IN_PRED_COMP_H

#include "prolog_bfs/wam/built_in_predicates/equals.h"
#include "prolog_bfs/wam/built_in_predicates/not_equals.h"
#include "prolog_bfs/wam/compiler/util/seen_register.h"
#include "prolog_bfs/wam/bfs_organizer/data/storage.h"

#include <utility>


namespace compiler{
    namespace preds{
        struct binary_built_in_pred{
            std::function<void(wam::executor&, size_t, size_t)> func;
            std::string name;

            binary_built_in_pred(
                    std::function<void(wam::executor &, size_t, size_t)> func, std::string name) :
                    func(std::move(func)), name(std::move(name)){}
        };

        node get_binary_pred(const std::string& name);


        std::unordered_map<wam::functor_view, std::vector<wam::rule>>
        compile_built_in_predicates(wam::storage &storage);
    }
}


#endif //PROLOG_BFS_BUILT_IN_PRED_COMP_H
