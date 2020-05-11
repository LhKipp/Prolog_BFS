//
// Created by leonhard on 11.05.20.
//


#include "arith_functor.h"
#include "arith_functor_impl.h"
#include <unordered_map>
#include <functional>
#include <cmath>
#include <wam/data/heap_reg.h>


const std::unordered_map<std::string, int> arith_func_to_index = {
        {"^", 0},
        {"+", 1},
        {"-", 2},
        {"*", 3},
        {"//", 4}
};

const int BINARY_FUNCS_BEGIN = 0;

const std::vector<std::function<double(double)>> unary_funcs = {
};

static const std::vector<std::function<double(double, double)>> binary_funcs = {
        std::function<double(double, double)>(&wam::arithmetic::pow),
        std::function<double(double, double)>(std::plus<double>{}),
        std::function<double(double, double)>(std::minus<double>{}),
        std::function<double(double, double)>(std::multiplies<double>{}),
        std::function<double(double, double)>(std::divides<double>{})
};


int wam::arithmetic::to_index(const std::string& arith_functor) {
    return arith_func_to_index.at(arith_functor);
}

int arithmetic::arity_of(const wam::heap_reg& func_reg) {
    return func_reg.index < BINARY_FUNCS_BEGIN ? 1 : 2;
}

const std::function<double(double)>& arithmetic::get_unary_func_of(const wam::heap_reg &eval_func_reg) {
    return unary_funcs[eval_func_reg.index];
}

const std::function<double(double, double)>& arithmetic::get_binary_func(const wam::heap_reg &eval_func_reg){
    return binary_funcs[eval_func_reg.index];
}
