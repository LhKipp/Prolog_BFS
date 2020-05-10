//
// Created by leonhard on 09.05.20.
//

#include "bfs_util.h"
#include <algorithm>

std::string append_dot_if_not_present(std::string query) {
    auto last_elem = std::find_if(query.rbegin(), query.rend(), [](char e){
       return !std::isspace(e);
    });
    //If the query is empty...
    if(last_elem == query.rend()){return query;}
    if(*last_elem == '.'){return query;}

    //we better append a .

    query.insert(last_elem.base() + 1, '.');
    return query;
}
