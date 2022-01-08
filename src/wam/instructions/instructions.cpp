//
// Created by leonhard on 27.07.19.
//

#include <stack>
#include "prolog_bfs/wam/instructions/instructions.h"
#include "prolog_bfs/wam/instructions/util/instructions_util.h"
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include "prolog_bfs/wam/built_in_predicates/arithmetic/util/arith_functor.h"

//#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif

void wam::put_structure(executor& executor, heap_reg reg, size_t regist_index){
#ifdef DEBUG
    std::cout << "put_structure with" << reg.type_as_str() << std::endl;
#endif
    executor.push_back(reg);
    executor.registers.at(regist_index) = {heap_tag::STR, (int)executor.heap_size() - 1};
}

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

void wam::get_structure(wam::executor& executor, wam::heap_reg structure, size_t x_reg){
#ifdef DEBUG
    std::cout << "get_structure with" << structure.type_as_str() << std::endl;
#endif
    size_t addr = wam::deref(executor, executor.registers.at(x_reg).get_heap_i());
    const heap_reg reg = executor.heap_at(addr);

    if (reg.type == heap_tag::REF) {
        //We bind a var from the query to a structure
        executor.push_back(structure);
        //exe.heap.size - 2 == H. The newly created ref cell will be bound
        executor.heap_modify(addr).bind_to((int)executor.heap_size() -1);
        executor.read_or_write = wam::mode::WRITE;
    } else if (reg == structure) {
        //This only has an effect if reg type == FUN or EVAL_FUN
        executor.S = addr + 1;
        executor.read_or_write = wam::mode::READ;
    } else {//Default case
        executor.set_failed();
    }
}

void wam::unify_structure(wam::executor &executor, heap_reg structure, size_t x_reg){
#ifdef DEBUG
    std::cout << "unify_structure with" << structure.type_as_str() << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            const size_t addr = wam::deref(executor, executor.S);
            const heap_reg to_be_matched = executor.heap_at(addr);
            if(to_be_matched.is_REF()){
                //bind the ref to a new cons
                executor.push_back(structure);
                executor.heap_modify(addr).bind_to((int)executor.heap_size() -1);
            }else if(to_be_matched == structure){
                //Everything fine just increase S at end of if else
            }else{
                executor.set_failed();
            }

            break;
        }
        case mode::WRITE: {
            executor.push_back(structure);
            executor.registers.at(x_reg) = heap_reg{heap_tag::STR, (int)executor.heap_size() -1};
            break;
        }
    }

    ++executor.S;
}

void wam::unify_variable(wam::executor &executor, size_t x_reg, short var_index) {
#ifdef DEBUG
    std::cout << "unify_variable" << std::endl;
#endif
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.registers.at(x_reg) = heap_reg{heap_tag::STR, (int) executor.S};
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
            executor.cur_permanent_registers().at(y_reg) = heap_reg{heap_tag::STR, (int)executor.S};
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
            const size_t heap_i = executor.registers[x_reg].get_heap_i();
            unify(executor, heap_i, executor.S);
            break;
        }
        case mode::WRITE: {
            //To stop binding chains we deref here and push back a reg pointing straight to the final reg
            const size_t heap_i = wam::deref(executor, executor.registers.at(x_reg).get_heap_i());
            const heap_reg reg = executor.heap_at(heap_i);
            switch(reg.type){
                case heap_tag::CONS:
                case heap_tag::INT:
                case heap_tag::REF:
                    executor.push_back(reg);
                    break;
                case heap_tag::FUN:
                case heap_tag::EVAL_FUN:
                    executor.push_back_STR(heap_i);
                    break;
                case heap_tag::STR:
                    assert(false);
            }
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
            const size_t heap_i = executor.cur_permanent_registers().at(y_reg).get_heap_i();
            unify(executor, heap_i, executor.S);
            break;
        }
        case mode::WRITE: {
            //To stop binding chains we deref here and push back a reg pointing straight to the final reg
            const size_t heap_i = wam::deref(executor, executor.cur_permanent_registers().at(y_reg).get_heap_i());
            const heap_reg reg = executor.heap_at(heap_i);
            switch(reg.type){
                case heap_tag::CONS:
                case heap_tag::INT:
                case heap_tag::REF:
                    executor.push_back(reg);
                    break;
                case heap_tag::FUN:
                case heap_tag::EVAL_FUN:
                    executor.push_back_STR(heap_i);
                    break;
                case heap_tag::STR:
                    assert(false);
            }
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
        size_t d1 = wam::deref(executor, PDL.top());
        PDL.pop();
        size_t d2 = wam::deref(executor, PDL.top());
        PDL.pop();
        if (d1 != d2) {
            const heap_reg reg1 = executor.heap_at(d1);
            const heap_reg reg2 = executor.heap_at(d2);

            if (reg1.is_REF()) {
                executor.heap_modify(d1).bind_to(d2);
            }else if(reg2.is_REF()){
                executor.heap_modify(d2).bind_to(d1);
            } else {
                if(reg1.type != reg2.type){
                    executor.set_failed();
                    return;
                }
                switch (reg1.type){
                    case heap_tag::INT: {
                        if (reg1.get_int_val() != reg2.get_int_val()) {
                            executor.set_failed();
                            return;
                        }
                        break;
                    }
                    case heap_tag::CONS: {
                        if (reg1.get_cons_i() != reg2.get_cons_i()) {
                            executor.set_failed();
                            return;
                        }
                        break;
                    }
                    case heap_tag::FUN:{
                        if(reg1.get_fun_i() != reg2.get_fun_i()){
                            executor.set_failed();
                            return;
                        }
                        for (int i = 1; i <= reg1.get_arity(); ++i) {
                            PDL.push(d1 + i);
                            PDL.push(d2 + i);
                        }
                        break;
                    }
                    case heap_tag::EVAL_FUN:{
                        if(reg1.get_eval_fun_i() != reg2.get_eval_fun_i()){
                            executor.set_failed();
                            return;
                        }
                        for (int i = 1; i <= reg1.get_arity(); ++i) {
                            PDL.push(d1 + i);
                            PDL.push(d2 + i);
                        }
                        break;
                    }
                    case heap_tag::REF:
                    case heap_tag::STR:
                    default: assert(false);
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

    executor.cur_permanent_registers().at(y_reg) = executor.heap_back(),
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
    std::cout << "with y_reg: " << y_reg << "; with a_reg: " << a_reg << std::endl;
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
    unify(executor, executor.registers.at(x_reg).get_heap_i(), executor.registers.at(a_reg).get_heap_i());
}

void wam::get_permanent_value(wam::executor &executor, size_t y_reg, size_t a_reg) {
#ifdef DEBUG
    std::cout << "get_permanent_value" << std::endl;
#endif
    //these should also be swapped feature/parser
    unify(executor, executor.cur_permanent_registers().at(y_reg).get_heap_i(), executor.registers.at(a_reg).get_heap_i());
}

void wam::call(wam::executor &old_executor, const functor_view &functor) {
#ifdef DEBUG
    std::cout << "call to: " << functor.name << std::endl;
#endif
    bfs_organizer *organizer = old_executor.get_organizer();
    //Everytime we call, we nearby finished the query. so we can inc the count
    organizer->runtime_stats.inc_queries_asked_count();

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


