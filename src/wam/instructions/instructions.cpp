//
// Created by leonhard on 27.07.19.
//

#include <stack>
#include "instructions.h"
#include "util/instructions_util.h"
#include "../bfs_organizer/bfs_organizer.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

void wam::put_structure(wam::executor &executor, const functor_view &functor, size_t regist_index) {
#ifdef DEBUG
    std::cout << "put_structure" << std::endl;
#endif
    executor.push_back_STR();
    executor.registers.at(regist_index) = executor.heap_back();
    executor.push_back_FUN(functor);
}

//void wam::put_list(wam::executor &executor, const size_t regist_index) {
//    executor.registers.at(regist_index] = regist{heap_tag ::LIS, executor.heap.size()};
//}

void wam::set_variable(wam::executor &executor, size_t x_reg) {
#ifdef DEBUG
    std::cout << "set_variable" << std::endl;
#endif
    executor.push_back_unbound_REF();
    executor.registers.at(x_reg) = executor.heap_back();
}

//TODO feature/tree add var name here
void wam::set_permanent_variable(wam::executor &executor, size_t y_reg) {
#ifdef DEBUG
    std::cout << "set_permanent_variable" << std::endl;
#endif
    executor.push_back_unbound_REF();
    executor.cur_permanent_registers().at(y_reg) = executor.heap_back();

}

void wam::set_value(wam::executor &executor, size_t x_reg) {
#ifdef DEBUG
    std::cout << "set_value" << std::endl;
#endif
    executor.push_back(executor.registers.at(x_reg));
}

void wam::set_permanent_value(wam::executor &executor, size_t y_reg) {
#ifdef DEBUG
    std::cout << "set_permanent_value" << std::endl;
#endif
    executor.push_back(executor.cur_permanent_registers().at(y_reg));
}

void wam::get_structure(wam::executor &executor, const functor_view &functor, size_t x_reg) {
#ifdef DEBUG
    std::cout << "get_structure" << std::endl;
#endif
    size_t addr;
    if (executor.registers.at(x_reg).is_REF()) {
        addr = deref(executor, executor.registers.at(x_reg));
    } else {//x_reg is a STR
        addr = executor.registers.at(x_reg).index;
    }

    const regist &reg = executor.heap_at(addr);

    if (reg.type == heap_tag::REF) {
        //We bind a var from the query to a functor
        executor.push_back_STR();
        executor.push_back_FUN(functor);

        //exe.heap.size - 2 == H. The newly created ref cell will be bound
        wam::bind(executor, addr, executor.heap_size() - 2);
        executor.read_or_write = wam::mode::WRITE;

    } else if (reg.type == heap_tag::FUN) {
        const functor_view &heap_reg = executor.functor_of(FUN_index{addr});
        if (heap_reg == functor) {
            executor.S = addr + 1;
            executor.read_or_write = wam::mode::READ;
        } else {
            executor.fail = true;
        }
    } else if (reg.type == heap_tag::STR) {
        throw int(3);
    } else {//Default case
        executor.fail = true;
    }
}


/*
 * See page 14 top
 */
//TODO feature/tree
void wam::bind(executor &exec, size_t address_a, size_t address_b) {
#ifdef DEBUG
    std::cout << "bind" << std::endl;
#endif
    if (exec.heap_at(address_a).index == address_a) {//If register a is unbound
        exec.heap_modify(address_a).index = address_b;
    } else {
        exec.heap_modify(address_b).index = address_a;
    }

}


void wam::unify_variable(wam::executor &executor, size_t x_reg) {
#ifdef DEBUG
    std::cout << "unify_variable" << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.registers.at(x_reg) = executor.heap_at(executor.S);
            break;
        }
        //TODO feature/tree add var name
        case mode::WRITE: {
            executor.push_back_unbound_REF();
            executor.registers.at(x_reg) = executor.heap_back();
            break;
        }
    }

    ++executor.S;
}

void wam::unify_permanent_variable(wam::executor &executor, size_t y_reg) {
#ifdef DEBUG
    std::cout << "unify_permanent_variable" << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.cur_permanent_registers().at(y_reg) = executor.heap_at(executor.S);
            break;
        }
        //TODO feature/tree add var name
        case mode::WRITE: {
            executor.push_back_unbound_REF();
            executor.cur_permanent_registers().at(y_reg) = executor.heap_back();
            break;
        }
    }

    ++executor.S;

}

void wam::unify_value(wam::executor &executor, size_t x_reg) {
#ifdef DEBUG
    std::cout << "unify_value" << std::endl;
#endif
    switch (executor.read_or_write) {
        case mode::READ: {
            if (executor.registers.at(x_reg).type == heap_tag::REF) {
                unify(executor, deref(executor, executor.registers.at(x_reg)), executor.S);
            } else {//x_reg is a STR
                unify(executor, executor.registers.at(x_reg).index - 1, executor.S);
            }
            break;
        }
        case mode::WRITE: {
            executor.push_back(executor.registers.at(x_reg));
            break;
        }
    }
    ++executor.S;
}

void wam::unify_permanent_value(wam::executor &executor, size_t y_reg) {
#ifdef DEBUG
    std::cout << "unify_permanent_value" << std::endl;
#endif
    switch (executor.read_or_write) {
        case mode::READ: {
            //TODO feature/tree shouldnt s be first addr, so that query / program
            if (executor.cur_permanent_registers().at(y_reg).is_REF()) {
                unify(executor, deref(executor, executor.cur_permanent_registers().at(y_reg)), executor.S);
            } else {//x_reg is a STR
                unify(executor, executor.cur_permanent_registers().at(y_reg).index - 1, executor.S);
            }
            break;
        }
        case mode::WRITE: {
            executor.push_back(executor.cur_permanent_registers().at(y_reg));
            break;
        }
    }
    ++executor.S;
}

void wam::unify(executor &executor, size_t addr_a, size_t addr_b) {
#ifdef DEBUG
    std::cout << "unify" << std::endl;
#endif
    std::stack<size_t> PDL;
    PDL.push(addr_b);
    PDL.push(addr_a);

    executor.fail = false;
    while (!(PDL.empty() || executor.fail)) {
        size_t d1, d2;
        if (executor.heap_at(PDL.top()).is_REF()){
            d1 = deref(executor, executor.heap_at(PDL.top()));
        } else {
            d1 = PDL.top();
        }
        PDL.pop();
        if (executor.heap_at(PDL.top()).is_REF()){
            d2 = deref(executor, executor.heap_at(PDL.top()));
        } else {
            d2 = PDL.top();
        }
        PDL.pop();

        if (executor.heap_at(d1).is_FUN()) {
            --d1;
        }
        if (executor.heap_at(d2).is_FUN()) {
            --d2;
        }

        if (d1 != d2) {
            const regist &reg1 = executor.heap_at(d1);
            const regist &reg2 = executor.heap_at(d2);

            if (reg1.is_REF() || reg2.is_REF()) {
                bind(executor, d1, d2);
            } else {
                //reg1 && reg2 are STR registers
                const functor_view &functor1 = executor.functor_of(FUN_index{reg1.index});
                const functor_view &functor2 = executor.functor_of(FUN_index{reg2.index});

                if (functor1 == functor2) {
                    for (int i = 1; i <= functor1.arity; ++i) {
                        PDL.push(reg1.index + i);
                        PDL.push(reg2.index + i);
                    }
                } else {
                    executor.fail = true;
                    return;
                }
            }
        }
    }
}

//TODO feature/tree add var name here
void wam::put_variable(wam::executor &executor, size_t x_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "put_variable" << std::endl;
#endif
    executor.push_back_unbound_REF();
    executor.registers.at(x_reg) = executor.heap_back();
    executor.registers.at(a_reg) = executor.heap_back();
}

//TODO feature/tree add var name here
void wam::put_permanent_variable(wam::executor &executor, size_t y_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "put_permanent_variable" << std::endl;
#endif
    //Is this instruction needed?
    executor.push_back_unbound_REF();

    executor.cur_permanent_registers().at(y_reg) = executor.heap_back();
    executor.registers.at(a_reg) = executor.heap_back();

}

void wam::put_value(wam::executor &executor, size_t x_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "put_value" << std::endl;
#endif
    executor.registers.at(a_reg) = executor.registers.at(x_reg);
}

void wam::put_permanent_value(wam::executor &executor, size_t y_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "put_permanent_value" << std::endl;
#endif
    executor.registers.at(a_reg) = executor.cur_permanent_registers().at(y_reg);
}

void wam::get_variable(wam::executor &executor, size_t x_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "get_variable" << std::endl;
#endif
    executor.registers.at(x_reg) = executor.registers.at(a_reg);
}

void wam::get_permanent_variable(wam::executor &executor, size_t y_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "get_permanent_variable" << std::endl;
#endif
    executor.cur_permanent_registers().at(y_reg) = executor.registers.at(a_reg);
}

void wam::get_value(wam::executor &executor, size_t x_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "get_value" << std::endl;
#endif
    //feature/parser TODO should be swapped
    unify(executor, executor.registers.at(x_reg).index, executor.registers.at(a_reg).index);
}

void wam::get_permanent_value(wam::executor &executor, size_t y_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "get_permanent_value" << std::endl;
#endif
    //these should also be swapped feature/parser
    unify(executor, executor.cur_permanent_registers().at(y_reg).index, executor.registers.at(a_reg).index);
}

void wam::call(wam::executor &old_executor, const functor_view &functor) {
#ifdef DEBUG
    std::cout << "call" << std::endl;
    std::cout << "call to: " << functor.name << std::endl;
#endif
    bfs_organizer *organizer = old_executor.get_organizer();

    if (!organizer->has_code_for(functor)) {
#ifdef DEBUG
        std::cout << "call failed" << std::endl;
#endif
        old_executor.fail = true;
        return;
    }

    auto range = organizer->program_code.equal_range(functor);
    auto old_exec_index = organizer->next_archive_index();
    std::for_each(range.first, range.second,
                  [&](auto &entries) {
                      executor new_executor{};
                      new_executor.set_parent(old_executor, old_exec_index);

                      std::for_each(entries.second.rbegin(), entries.second.rend(),
                                    [&](wam::term_code &term_code) {
                                        new_executor.term_codes.push(&term_code);
                                    });
                      organizer->executors.push_back(std::move(new_executor));
                  });
    old_executor.clear();
    organizer->archive_node_exec(std::move(old_executor));
}

void wam::proceed(wam::executor &old_exec) {
#ifdef DEBUG
    std::cout << "proceed" << std::endl;
#endif
    //The old_exec has unified a rule head atom / fact with an query
    //To preserve the var_heap_subst in old_exec and the state after
    //unification we start a new executor who continues with the rest of
    //the instructions
    bfs_organizer *organizer = old_exec.get_organizer();

    const auto archive_index = organizer->next_archive_index();
    executor new_executor{};
    new_executor.set_parent(std::move(old_exec), archive_index);
    organizer->archive_node_exec(std::move(old_exec));
    organizer->executors.push_back(std::move(new_executor));
}

void wam::allocate(wam::executor &executor, size_t permanent_var_count) {
#ifdef DEBUG
    std::cout << "allocate" << std::endl;
#endif
    executor.environments.push(environment{permanent_var_count});
}

void wam::deallocate(wam::executor &executor) {
#ifdef DEBUG
    std::cout << "deallocate" << std::endl;
#endif
    executor.environments.pop();
    bfs_organizer *organizer = executor.get_organizer();
    organizer->executors.push_back(std::move(executor));
}

void wam::point_var_reg_substs_to_heap(wam::executor &executor) {
#ifdef DEBUG
    std::cout << "point_var_reg_substs_to_heap" << std::endl;
#endif
    //This operation could theoretically be done later, when finding
    //Var bindings. But doing it now allows us to clear register and permanent registers
    //From dead exec
    auto const& var_reg_substs = executor.solves_term_code->get_substitutions();

    executor.substitutions.reserve(var_reg_substs.size());

    std::transform(var_reg_substs.begin(),
             var_reg_substs.end(),
             std::back_inserter(executor.substitutions),
           [&](const var_reg_substitution& reg_sub){
               size_t heap_index = reg_sub.is_permanent_register ?
                                   executor.environments.top().permanent_registers.at(reg_sub.register_index).index
                                  : executor.registers.at(reg_sub.register_index).index;

               return var_heap_substitution(
                        reg_sub.var_name,
                        heap_index
               );
    });
}
