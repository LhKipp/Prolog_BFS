//
// Created by leonhard on 27.07.19.
//

#include <stack>
#include "instructions.h"
#include "util/instructions_util.h"
#include "../bfs_organizer/bfs_organizer.h"
#include "../data/rule.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

void wam::put_expr(executor &executor, int expr_index, size_t regist_index) {
#ifdef DEBUG
    std::cout << "put_int" << std::endl;
#endif
    executor.push_back_expr(expr_index);
    executor.registers.at(regist_index) = executor.heap_back();

}

void wam::put_int(wam::executor &executor, int value, size_t regist_index) {
#ifdef DEBUG
    std::cout << "put_int" << std::endl;
#endif
    executor.push_back_int(value);
    executor.registers.at(regist_index) = executor.heap_back();
}

void wam::put_structure(wam::executor &executor, int functor_index, size_t regist_index) {
#ifdef DEBUG
    std::cout << "put_structure" << std::endl;
#endif
    executor.push_back_STR();
    executor.registers.at(regist_index) = executor.heap_back();
    executor.push_back_FUN(functor_index);
}

//void wam::put_list(wam::executor &executor, const size_t regist_index) {
//    executor.registers.at(regist_index] = regist{heap_tag ::LIS, executor.heap.size()};
//}

void wam::set_variable(wam::executor &executor, size_t x_reg, short var_index) {
#ifdef DEBUG
    std::cout << "set_variable" << std::endl;
#endif
    executor.push_back_unbound_REF(var_index);
    executor.registers.at(x_reg) = executor.heap_back();
}

void wam::set_permanent_variable(wam::executor &executor, size_t y_reg, short var_index) {
#ifdef DEBUG
    std::cout << "set_permanent_variable" << std::endl;
#endif
    executor.push_back_unbound_REF(var_index);
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

void wam::get_int(wam::executor &executor, int value, size_t x_reg) {
#ifdef DEBUG
    std::cout << "get_int" << std::endl;
#endif
    regist reg;
    size_t heap_addr;
    if(executor.registers.at(x_reg).is_REF()) {
        heap_addr = deref(executor, executor.registers.at(x_reg));
        reg = executor.heap_at(heap_addr);
    }else{ //reg is int reg
        reg = executor.registers.at(x_reg);
    }

    if (reg.is_REF()) {
        //We bind a var from the query to a int
        executor.push_back_int(value);

        //exe.heap.size - 1 == H. The var will be bound to the new int
        executor.heap_modify(heap_addr).bind_to((int)executor.heap_size() - 1);
        executor.read_or_write = wam::mode::WRITE;
    } else if (reg.is_INT()) {
        if (reg.get_int_val() == value) {
            executor.S = heap_addr + 1;
            executor.read_or_write = wam::mode::READ;
        } else {
            executor.set_failed();
        }
    } else {//Default case if reg.is_FUN() || reg.is_STR() || reg.is_LIST()
        executor.set_failed();
    }
}

void wam::get_structure(wam::executor &executor, int functor_index, size_t x_reg) {
#ifdef DEBUG
    std::cout << "get_structure" << std::endl;
#endif
    size_t addr;
    if (executor.registers.at(x_reg).is_REF()) {
        addr = deref(executor, executor.registers.at(x_reg));
    } else {//x_reg is a STR
        addr = executor.registers.at(x_reg).index;
    }

    //TODO heap_at returns no ref!!! FIX NEEDED, remove & why does it even compile?
    const regist &reg = executor.heap_at(addr);

    if (reg.type == heap_tag::REF) {
        //We bind a var from the query to a functor
        executor.push_back_STR();
        executor.push_back_FUN(functor_index);

        //exe.heap.size - 2 == H. The newly created ref cell will be bound
        wam::bind(executor, addr, executor.heap_size() - 2);
        executor.read_or_write = wam::mode::WRITE;

    } else if (reg.type == heap_tag::FUN) {
        const functor_view &heap_reg = executor.functor_of(FUN_index{(int)addr});
        const functor_view functor = executor.functor_of(Storage_FUN_index{functor_index});
        if (heap_reg == functor) {
            executor.S = addr + 1;
            executor.read_or_write = wam::mode::READ;
        } else {
            executor.set_failed();
        }
    } else if (reg.type == heap_tag::STR) {
        //This should never happen
        throw int(3);
    } else {//Default case
        executor.set_failed();
    }
}


/*
 * See page 14 top
 */
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


void wam::unify_variable(wam::executor &executor, size_t x_reg, short var_index) {
#ifdef DEBUG
    std::cout << "unify_variable" << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.registers.at(x_reg) = executor.heap_at(executor.S);
            break;
        }
        case mode::WRITE: {
            executor.push_back_unbound_REF(var_index);
            executor.registers.at(x_reg) = executor.heap_back();
            break;
        }
    }

    ++executor.S;
}

void wam::unify_permanent_variable(wam::executor &executor, size_t y_reg, short var_index) {
#ifdef DEBUG
    std::cout << "unify_permanent_variable" << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.cur_permanent_registers().at(y_reg) = executor.heap_at(executor.S);
            break;
        }
        case mode::WRITE: {
            executor.push_back_unbound_REF(var_index);
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

    while (!(PDL.empty() || executor.failed())) {
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
                    executor.set_failed();
                    return;
                }
            }
        }
    }
}

void wam::put_variable(wam::executor &executor, size_t x_reg, size_t a_reg, short var_index) {
#ifdef DEBUG
    std::cout << "put_variable" << std::endl;
#endif
    executor.push_back_unbound_REF(var_index);
    executor.registers.at(x_reg) = executor.heap_back();
    executor.registers.at(a_reg) = executor.heap_back();
}

void wam::put_permanent_variable(wam::executor &executor, size_t y_reg, size_t a_reg, short var_index) {
#ifdef DEBUG
    std::cout << "put_permanent_variable" << std::endl;
#endif
    executor.push_back_unbound_REF(var_index);

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
    //feature/parser TODO should be swapped ???
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
    std::cout << "call to: " << functor.name << std::endl;
#endif
    bfs_organizer *organizer = old_executor.get_organizer();

    if (!organizer->has_code_for(functor)) {
#ifdef DEBUG
        std::cout << "call failed" << std::endl;
#endif
        old_executor.set_failed();
        return;
    }

    auto& rules = organizer->program_code[functor];
    std::for_each(rules.begin(), rules.end(),
                  [&](wam::rule& rule) {
#ifdef DEBUG
        std::cout << "found rule at line_begin: "
        << rule.atoms()[0].get_code_info().line_begin
        << " "
        << rule.atoms()[0].get_code_info().value
        << std::endl;
#endif
                      executor new_executor{old_executor.term_codes.size() - 1 + rule.atoms().size()};
//                      Copy the term_codes
                      auto parent_codes_end = std::copy(old_executor.term_codes.begin(),
                              old_executor.term_codes.end() -1,
                              new_executor.term_codes.begin());
                      std::transform(rule.atoms().rbegin(),
                                     rule.atoms().rend(),
                                     parent_codes_end,
                                     [](wam::compiled_atom& term_code){return &term_code;});
                      new_executor.set_parent(old_executor);


                      old_executor.push_back_child(std::move(new_executor));
                      organizer->executors.push_back(&old_executor.get_last_child());
                  });
    old_executor.clear();
    old_executor.set_archived();
}

void wam::proceed(wam::executor &old_exec) {
#ifdef DEBUG
    std::cout << "proceed" << std::endl;
#endif

    if(old_exec.check_success()){
        //No more work, eventhough there might be some deallocs.
        //So we dealloc in clear
        old_exec.clear();
        return; //No more work
    }

    //The old_exec has unified a rule head atom / fact with an query
    //To preserve the var_heap_subst in old_exec and the state after
    //unification we start a new executor who continues with the rest of
    //the instructions
    bfs_organizer *organizer = old_exec.get_organizer();

    executor new_executor{};
    new_executor.move_from_parent(old_exec);
    old_exec.push_back_child(std::move(new_executor));
    organizer->executors.push_back(&old_exec.get_last_child());
}

void wam::allocate(wam::executor &executor, size_t permanent_var_count) {
#ifdef DEBUG
    std::cout << "allocate" << std::endl;
    std::cout << "exec environment size before: " << executor.environments.size() << std::endl;
#endif
    executor.environments.emplace_back(permanent_var_count);
}

void wam::deallocate(wam::executor &executor) {
#ifdef DEBUG
    std::cout << "deallocate" << std::endl;
    std::cout << "exec environment size before: " << executor.environments.size() << std::endl;
#endif
    assert(!executor.environments.empty());
    executor.environments.pop_back();

    //This executor finished his compiled_atom. But storing executors only doing deallocates is
    //unecessary, so we give him a new task, through term_codes.pop_back and inserting him in
    //executors list
    executor.term_codes.pop_back();
    bfs_organizer *organizer = executor.get_organizer();
    organizer->executors.push_back(&executor);
}


