//
// Created by leonhard on 09.05.20.
//
#include "prolog_bfs/wam/bfs_organizer/data/result.h"
#include <sstream>
#include "prolog_bfs/util/vector_util.h"

std::string wam::result::get_answer_as_str() const {
    auto answer = get_answer();

    if(!answer){
        return "false";
    }

    //If it was an easy unification without variables
    if(answer->empty()){
        return "true";
    }

    std::stringstream ans;
    for(int i = 0; i < answer->size() -1; i++){
        ans << answer->at(i) << ' ';
    }
    ans << answer->back();

    return ans.str();
}
