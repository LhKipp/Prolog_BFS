//
// Created by leonhard on 11.05.20.
//


#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/arith_functor.h"
#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/arith_functor_impl.h"
#include <boost/bimap.hpp>
#include <unordered_map>
#include <functional>
#include <cmath>
#include "prolog_bfs/wam/data/heap_reg.h"


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

int wam::arithmetic::arity_of(int func_i) {
    return func_i < BINARY_FUNCS_BEGIN ? 1 : 2;
}
int wam::arithmetic::arity_of(const wam::heap_reg& func_reg) {
    return func_reg.index < BINARY_FUNCS_BEGIN ? 1 : 2;
}

const std::function<double(double)>& wam::arithmetic::get_unary_func_of(const wam::heap_reg &eval_func_reg) {
    return unary_funcs[eval_func_reg.index];
}

const std::function<double(double, double)>& wam::arithmetic::get_binary_func(const wam::heap_reg &eval_func_reg){
    return binary_funcs[eval_func_reg.index];
}

wam::functor_view wam::arithmetic::functor_of(size_t func_i) {
    std::string name;
    for(const auto& elem : arith_func_to_index){
        if(elem.second == func_i){
            name = elem.first;
            break;
        }
    }
    return functor_view{name, arity_of(func_i)};
}
