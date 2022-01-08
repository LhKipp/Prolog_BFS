//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_GENERAL_UTILITY_H
#define PROLOG_BFS_GENERAL_UTILITY_H

#include <string>
#include <vector>
#include <optional>
#include <map>
#include "prolog_bfs/wam/data/var_binding.h"

std::string vec_to_string(const std::vector<std::string> &vec);

void has_all_of_these(std::optional<std::vector<wam::var_binding>> ans,
        std::initializer_list<std::string> substs);
#endif //PROLOG_BFS_GENERAL_UTILITY_H
