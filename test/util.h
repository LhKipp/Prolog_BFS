//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_UTIL_H
#define PROLOG_BFS_UTIL_H

#include <string>
#include <vector>
#include <numeric>

std::string vec_to_string(const std::vector<std::string> &vec){
    return std::accumulate(std::begin(vec), std::end(vec), std::string(),
                           [](std::string &ss, const std::string &s)
                           {
                               return ss.empty() ? s : ss + '\n' + s;
                           });
}
#endif //PROLOG_BFS_UTIL_H
