//
// Created by leonhard on 29.07.19.
//

#include "instructions_util.h"
#include "../../data/regist.h"
#include <vector>

/*
 * Assumes init_regist is a REF x_reg
 * Returns a index to a register the init_regist points to.
 * The heap register will be either FUN or REF
 */
size_t wam::deref(const std::vector<regist> & store, const regist& init_regist) {

    size_t last_reg_index = init_regist.index;
    const regist *cur_regist = &store[last_reg_index];
    while (cur_regist->type == heap_tag::REF && last_reg_index != cur_regist->index) {
        //If the Ref cell doesnt reference itself, it will get dereferenced
        last_reg_index = cur_regist->index;
        cur_regist = &store[cur_regist->index];
    }


    return last_reg_index;
}
