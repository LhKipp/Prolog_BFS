//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_INSTRUCTIONS_H
#define PROLOG_BFS_INSTRUCTIONS_H

#include "../executor/executor.h"

namespace wam {
    void put_structure(executor& executor, const functor_view& functor, size_t regist_index);

//    void put_list(executor& executor, const size_t regist_index);

    void set_variable(executor& executor , size_t x_reg);
    void set_permanent_variable(executor& executor , size_t y_reg);

    void set_value(executor& executor,size_t x_reg);
    void set_permanent_value(executor& executor, size_t y_reg);

    void get_structure(executor& executor,const functor_view& functor, size_t x_reg);

    void unify_variable(executor& executor,  size_t x_reg);
    void unify_permanent_variable(executor& executor, size_t y_reg);

    void unify_value(executor& executor, size_t x_reg);
    void unify_permanent_value(executor& executor, size_t y_reg);

    void bind(executor& exec, size_t address_a,size_t address_b);

    void unify(executor& executor, size_t addr_a, size_t addr_b);

    void put_variable(executor& executor, size_t x_reg, size_t a_reg);
    void put_permanent_variable(executor& executor, size_t y_reg, size_t a_reg);

    void put_value(executor& executor, size_t x_reg, size_t a_reg);
    void put_permanent_value(executor& executor, size_t y_reg, size_t a_reg);

    void get_variable(executor& executor, size_t x_reg, size_t a_reg);
    void get_permanent_variable(executor& executor, size_t y_reg, size_t a_reg);

    void get_value(executor& executor, size_t x_reg, size_t a_reg);
    void get_permanent_value(executor& executor, size_t y_reg, size_t a_reg);

    void call(wam::executor &old_executor, const functor_view &functor, bool from_original_query);
    void proceed(executor& executor);

    void allocate(executor& executor, size_t permanent_var_count);
    void deallocate(executor& executor);
}

#endif //PROLOG_BFS_INSTRUCTIONS_H
