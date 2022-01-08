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

        short inline functor_index_of(const functor_view& func){
            auto search = functor_index_map.find(func);
            //If we have seen this functor already
            if (search != functor_index_map.end()) {
                return search->second;
            } else {
                const auto index = functors.size();
                functors.push_back(func);
                functor_index_map.insert({func, index});
                return index;
            }
        }

        short inline push_back_var(std::string name){
            variables.emplace_back(name);
            return static_cast<short>(variables.size() - 1);
        }

        void inline clear_memory() {
            //Global storage for all executors
            std::vector<var>().swap(variables);
            std::vector<functor_view>().swap(functors);
            std::unordered_map<functor_view, size_t>().swap(functor_index_map);
        }
    };
}
#endif //PROLOG_BFS_STORAGE_H
