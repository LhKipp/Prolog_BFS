//
// Created by leonhard on 24.07.19.
//

#include "executor.h"

#include "../bfs_organizer/bfs_organizer.h"
wam::functor_view &wam::executor::functor_of(FUN_index FUN_index) {
    return organizer->functors.operator[](heap_at(FUN_index.get()).index);
}
const wam::functor_view &wam::executor::functor_of(FUN_index FUN_index) const {
    return organizer->functors.operator[](heap_at(FUN_index.get()).index);
}

size_t wam::executor::index_of(const functor_view &functor) const{
    auto search = organizer->functor_index_map.find(functor);
    //If we have seen this functor already
    if (search != organizer->functor_index_map.end()) {
        return search->second;
    } else {
        const auto index = organizer->functors.size();
        organizer->functors.push_back(functor);
        organizer->functor_index_map.operator[](functor)= index;
        return index;
    }
}


