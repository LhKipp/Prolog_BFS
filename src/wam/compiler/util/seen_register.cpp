//
// Created by leonhard on 08.05.20.
//

#include "prolog_bfs/wam/compiler/util/seen_register.h"
#include "prolog_bfs/wam/compiler/util/node.h"

#include <cassert>

wam::helper::seen_register::seen_register(const node &var) {
    assert(var.is_variable());
    if(var.is_permanent()){
        type = register_type ::Y_REG;
        index = var.get_y_reg();
    }else{
        type = register_type ::X_REG;
        index = var.get_x_reg();
    }
}
