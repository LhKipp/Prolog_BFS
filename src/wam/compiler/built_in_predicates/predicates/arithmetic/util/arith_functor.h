//
// Created by leonhard on 11.05.20.
//

#ifndef PROLOG_BFS_ARITH_FUNCTOR_H
#define PROLOG_BFS_ARITH_FUNCTOR_H

#include <string_view>
#include <wam/data/heap_reg.h>
#include <functional>

namespace wam::arithmetic{

    int to_index(const std::string& arith_functor);

    int arity_of(const wam::heap_reg& eval_func_reg);

    const std::function<double(double)>& get_unary_func_of(const wam::heap_reg& eval_func_reg);
    const std::function<double(double, double)>& get_binary_func(const wam::heap_reg& eval_func_reg);

    inline int get_int_div_i(){
        return 4;
    }
    inline int get_div_i(){
        return 4;
    }

};

#endif //PROLOG_BFS_ARITH_FUNCTOR_H
