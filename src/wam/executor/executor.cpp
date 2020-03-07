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
//        organizer->functor_index_map.operator[](functor)= index;
        organizer->functor_index_map.insert({functor, index});
        return index;
    }
}

wam::regist wam::executor::heap_back() const {
    assert(heap_size() > 0);
    if(heap.empty()){
        assert(has_parent());
        return parent->heap_back();
    }else{
        return heap.back();
    }
}

wam::regist &wam::executor::heap_modify(size_t index) {
    //Assert that the index is within heap
    assert(index < heap_size());
    //If within this executor
    if(heap_start_index <= index && index < heap_size()){
        return heap.at(index - heap_start_index);
    }else{//index is within parents range, which this executor is not allowed to change
        //If this executor has overwritten the parent heap at index, return the change
        auto change = changes_to_parent.find(index);
        if(change != changes_to_parent.end()){
            return change->second;
        }else{
            //This executor has not overwritten the parent heap
            //So we give back a regist stored as a local change to parent
            //First copy the parent regist
            changes_to_parent.insert({index, parent->heap_at(index)});
            return changes_to_parent.at(index);
        }
    }
}

wam::regist wam::executor::heap_at(size_t index) const {
    //Assert that the index is within heap
    assert(index < heap_size());
    //If within child
    if(heap_start_index <= index && index < heap_size()){
        return heap.at(index - heap_start_index);
    }else{//index is within parents range
        //If this executor has overwritten the parent heap at index, return the change
        auto change = changes_to_parent.find(index);
        if(change != changes_to_parent.end()){
            return change->second;
        }else{
            //This executor has not overwritten the parent heap --> search in parent heap
            assert(has_parent());
            return parent->heap_at(index);
            //DEBUG
            //return regist{heap_tag::REF, 0};
        }
    }
}
