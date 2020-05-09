//
// Created by leonhard on 09.05.20.
//

#include <wam/compiler/util/node_iteration.h>
#include <wam/compiler/parser/parser_error.h>
#include <wam/compiler/error/compiler_error.h>
#include "undefined_var.h"

void compiler::check_and_throw_undefined_var(
        const node& atom,
        const node &expr,
        const std::unordered_map<wam::helper::seen_register, bool> &seen_registers) {
    using wam::helper::seen_register;
    bfs_order(expr, true, [&](const node* n){
        if(!n->is_variable()) return;
        seen_register reg{*n};
        if(seen_registers.find(reg) == seen_registers.end()){
            throw compiler::error{
                ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED,
                atom.code_info,
                "Any variable inside an expression must be defined before."
            };
        }
    });

}
