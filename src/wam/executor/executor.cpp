//
// Created by leonhard on 24.07.19.
//

#include "prolog_bfs/wam/executor/executor.h"

#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

wam::functor_view &wam::executor::functor_of(FUN_index FUN_index) {
    return organizer->storage.functors.operator[](heap_at(FUN_index.get()).index);
}
const wam::functor_view &wam::executor::functor_of(FUN_index FUN_index) const {
    return organizer->storage.functors.operator[](heap_at(FUN_index.get()).index);
}

size_t wam::executor::storage_index_of(const functor_view &functor) const{
    return organizer->storage.functor_index_of(functor);
}


wam::heap_reg wam::executor::heap_back() const {
    assert(heap_size() > 0);
    if(heap.empty()){
        assert(has_parent());
        return parent->heap_back();
    }else{
        return heap.back();
    }
}

wam::heap_reg &wam::executor::heap_modify(size_t index) {
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
            //So we give back a heap_reg stored as a local change to parent
            //First copy the parent heap_reg
            changes_to_parent.insert({index, parent->heap_at(index)});
            return changes_to_parent.at(index);
        }
    }
}

wam::heap_reg wam::executor::heap_at(size_t index) const {
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
        }
    }
}

const wam::functor_view &wam::executor::functor_of(Storage_FUN_index storage_fun_index) const {
    return organizer->storage.functors[storage_fun_index.get()];
}
wam::functor_view &wam::executor::functor_of(Storage_FUN_index indx) {
    return organizer->storage.functors[indx.get()];
}

std::string executor::var_name_of(Storage_Var_index var_index) {
    return organizer->storage.variables[var_index.get()].name;
}

