//
// Created by leonhard on 29.07.19.
//

#include "instructions_util.h"
#include "wam/data/heap_reg.h"
#include "../../executor/executor.h"
#include <vector>
#include <cassert>

#ifdef DEBUG
#include <iostream>
#endif

/*
 * Returns a index to a register the init_regist points to.
 */
size_t wam::deref(const executor & exec, const heap_reg& init_regist) {
#ifdef DEBUG
    std::cout << "deref" << std::endl;
#endif
    return wam::deref(exec, init_regist.index);
}

/*
 * Returns a index to a register the ref or Str under heap_addr points to.
 */
size_t wam::deref(const executor & exec, const size_t heap_addr) {
#ifdef DEBUG
    std::cout << "deref" << std::endl;
#endif

    size_t last_reg_index = heap_addr;
    heap_reg cur_regist = exec.heap_at(last_reg_index);
    while ((cur_regist.type == heap_tag::REF
            || cur_regist.type == heap_tag::STR)
           //If the Ref cell doesnt reference itself, it will get dereferenced
           && last_reg_index != cur_regist.index) {
        last_reg_index = cur_regist.index;
        cur_regist = exec.heap_at(cur_regist.index);
    }

    return last_reg_index;
}

wam::heap_reg wam::derefed_reg(const executor &exec, const heap_reg &init_regist) {
    auto deref_i = deref(exec, init_regist);
    return exec.heap_at(deref_i);
}


wam::heap_reg& wam::derefed_reg_modify(executor &exec, const heap_reg &init_regist) {
    auto deref_i = deref(exec, init_regist);
    return exec.heap_modify(deref_i);
}

wam::heap_reg wam::derefed_reg(const executor &exec, size_t heap_i) {
    auto deref_i = deref(exec, heap_i);
    return exec.heap_at(deref_i);
}


wam::heap_reg& wam::derefed_reg_modify(executor &exec, size_t heap_i) {
    auto deref_i = deref(exec, heap_i);
    return exec.heap_modify(deref_i);
}
