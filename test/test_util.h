//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_BFS_UTIL_H
#define PROLOG_BFS_UTIL_H

#include <string>
#include <vector>
#include <map>
#include <wam/data/var_binding.h>

std::string vec_to_string(const std::vector<std::string> &vec);

bool has_all_of_these(std::optional<std::vector<wam::var_binding>> ans,
        std::initializer_list<std::string> substs);
#endif //PROLOG_BFS_BFS_UTIL_H
