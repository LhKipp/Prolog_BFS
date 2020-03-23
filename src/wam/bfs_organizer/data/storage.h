//
// Created by leonhard on 19.03.20.
//

#ifndef PROLOG_BFS_STORAGE_H
#define PROLOG_BFS_STORAGE_H

#include <unordered_map>
#include <vector>
#include "../../data/functor_view.h"

namespace wam{
    struct var{
        std::string name;

        var(std::string name):name(std::move(name)){}
    };

    struct storage{
        //Global storage for all executors
        std::unordered_map<functor_view, size_t> functor_index_map;

        std::vector<functor_view> functors;

        std::vector<var> variables;

        short inline push_back_var(std::string name){
            variables.emplace_back(var{name});
            return static_cast<short>(variables.size() - 1);
        }
    };
}
#endif //PROLOG_BFS_STORAGE_H
