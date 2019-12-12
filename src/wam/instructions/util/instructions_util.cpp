//
// Created by leonhard on 29.07.19.
//

#include "instructions_util.h"
#include "../../data/regist.h"
#include "../../executor/executor.h"
#include <vector>
#include <cassert>

#define DEBUG 1
#ifdef DEBUG
#include <iostream>
#endif

/*
 * Assumes init_regist is a REF x_reg
 * Returns a index to a register the init_regist points to.
 * The heap register will be either FUN or REF
 */
size_t wam::deref(const executor & exec, const regist& init_regist) {
#ifdef DEBUG
    std::cout << "deref" << std::endl;
#endif

    size_t last_reg_index = init_regist.index;
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

    //The register is a REF cell pointing to itself
    assert(cur_regist.is_REF());
    return last_reg_index;
}
