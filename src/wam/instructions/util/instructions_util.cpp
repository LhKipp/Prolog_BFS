//
// Created by leonhard on 29.07.19.
//

#include "instructions_util.h"
#include "../../data/regist.h"
#include "../../executor/executor.h"
#include <vector>
#include <cassert>

#ifdef DEBUG
#include <iostream>
#endif

/*
 * Returns a index to a register the init_regist points to.
 */
size_t wam::deref(const executor & exec, const regist& init_regist) {
#ifdef DEBUG
    std::cout << "deref" << std::endl;
#endif
    assert(init_regist.is_REF());
    return wam::deref(exec, init_regist.index);
}

/*
 * Returns a index to a register the ref under heap_addr points to.
 */
size_t wam::deref(const executor & exec, const size_t heap_addr) {
#ifdef DEBUG
    std::cout << "deref" << std::endl;
#endif

    size_t last_reg_index = heap_addr;
    regist cur_regist = exec.heap_at(last_reg_index);

    while (cur_regist.type == heap_tag::REF && last_reg_index != cur_regist.index) {
        //If the Ref cell doesnt reference itself, it will get dereferenced
        last_reg_index = cur_regist.index;
        cur_regist = exec.heap_at(cur_regist.index);
    }

    //If the cur_reg is a STR reg, we return the FUN reg
    if(cur_regist.is_STR()) {
        return cur_regist.index;
    }

    assert(cur_regist.is_REF() || cur_regist.is_INT());
    return last_reg_index;
}

regist wam::derefed_reg(const executor &exec, const regist &init_regist) {
    auto deref_i = deref(exec, init_regist);
    return exec.heap_at(deref_i);
}

regist derefed_reg(const executor &exec, size_t heap_i) {
    auto deref_i = deref(exec, heap_i);
    return exec.heap_at(deref_i);
}

regist& wam::derefed_reg_modify(executor &exec, const regist &init_regist) {
    auto deref_i = deref(exec, init_regist);
    return exec.heap_modify(deref_i);
}

regist& derefed_reg_modify(executor &exec, size_t heap_i) {
    auto deref_i = deref(exec, heap_i);
    return exec.heap_modify(deref_i);
}
