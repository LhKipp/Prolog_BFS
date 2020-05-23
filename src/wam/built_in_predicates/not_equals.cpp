//
// Created by leonhard on 28.04.20.
//

#include <wam/instructions/util/instructions_util.h>
#include "not_equals.h"
#include "equals.h"

void wam::preds::not_equals_check(wam::executor &executor, size_t lhs_x_reg_i, size_t rhs_x_reg_i) {
#ifdef DEBUG
    std::cout << "equals_check" << std::endl;
#endif
    //The default assignment operator is deleted, but a whole copy is not needed anyways
    equals_check(executor, lhs_x_reg_i, rhs_x_reg_i);
    //if not not_equals
    if(executor.failed()){
        //if they were not equal
        executor.set_state(EXEC_STATE::RUNNING);
    }else{
        //if they were equal
        executor.set_failed();
    }
}

