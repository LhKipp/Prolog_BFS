//
// Created by leonhard on 27.07.19.
//

#include <stack>
#include <iostream>
#include "instructions.h"
#include "util/instructions_util.h"
#include "../bfs_organizer/bfs_organizer.h"

void wam::put_structure(wam::executor &executor, const functor_view& functor, size_t x_reg) {
    executor.heap.emplace_back(heap_tag::STR, executor.heap.size() + 1);
    executor.registers[x_reg] = executor.heap.back();

    size_t functor_reg_index = executor.index_of(functor);
    executor.heap.emplace_back(heap_tag::FUN, functor_reg_index);
}

void wam::set_variable(wam::executor &executor, size_t x_reg) {
    executor.heap.emplace_back(heap_tag::REF, executor.heap.size());
    executor.registers[x_reg] = executor.heap.back();
}

void wam::set_value(wam::executor &executor, size_t x_reg) {
    executor.heap.emplace_back(executor.registers[x_reg]);
}

void wam::get_structure(wam::executor &executor, const functor_view &functor, size_t x_reg) {
    size_t addr;
    if(executor.registers.at(x_reg).is_REF()){
        addr = deref(executor.heap, executor.registers.at(x_reg));
    }else {//x_reg is a STR
        addr = executor.registers.at(x_reg).index;
    }

    const regist &reg = executor.heap.at(addr);

    if (reg.type == heap_tag::REF) {
        //We bind a var from the query to a functor
        executor.heap.emplace_back(heap_tag::STR, executor.heap.size() + 1);
        executor.heap.emplace_back(heap_tag::FUN, executor.index_of(functor));

        //exe.heap.size - 2 == H. The newly created ref cell will be bound
        wam::bind(executor.heap, addr, executor.heap.size() - 2);
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
void wam::bind(std::vector<wam::regist> &store, size_t address_a, size_t address_b) {
    if (store[address_a].index == address_a) {//If register a is unbound
        store[address_a].index = address_b;
    } else {
        store[address_b].index = address_a;
    }

}


void wam::unify_variable(wam::executor &executor, size_t x_reg) {
    switch (executor.read_or_write) {
        case wam::mode::READ: {
            executor.registers[x_reg] = executor.heap[executor.S];
            break;
        }
        case mode::WRITE: {
            executor.heap.emplace_back(heap_tag::REF, executor.heap.size());
            executor.registers[x_reg] = executor.heap.back();
            break;
        }
    }

    ++executor.S;
}

void wam::unify_value(wam::executor &executor, size_t x_reg) {
    switch (executor.read_or_write) {
        case mode::READ: {
            if(executor.registers[x_reg].type == heap_tag::REF){
                unify(executor, deref(executor.heap, executor.registers[x_reg]), executor.S);
            }else{//x_reg is a STR
                unify(executor, executor.registers[x_reg].index - 1, executor.S);
            }
            break;
        }
        case mode::WRITE: {
            executor.heap.emplace_back(executor.registers[x_reg]);
            break;
        }
    }

    ++executor.S;
}

void wam::unify(executor &executor, size_t addr_a, size_t addr_b) {
    std::stack<size_t> PDL;
    PDL.push(addr_b);
    PDL.push(addr_a);

    executor.fail = false;
    while (!(PDL.empty() || executor.fail)) {
        size_t d1, d2;
        if(is_REF(executor.heap, PDL.top())){
            d1 = deref(executor.heap, executor.heap[PDL.top()]);
        }else{
            d1 = PDL.top();
        }
        PDL.pop();
        if(is_REF(executor.heap, PDL.top())){
            d2 = deref(executor.heap, executor.heap[PDL.top()]);
        }else{
            d2 = PDL.top();
        }
        PDL.pop();

        if(executor.heap.at(d1).is_FUN()){
            --d1;
        }
        if(executor.heap.at(d2).is_FUN()){
            --d2;
        }

        if (d1 != d2) {
            const regist &reg1 = executor.heap[d1];
            const regist &reg2 = executor.heap[d2];

            if (reg1.is_REF() || reg2.is_REF()) {
                bind(executor.heap, d1, d2);
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

void wam::put_variable(wam::executor &executor, size_t x_reg, size_t a_reg) {
    executor.push_back_unbound_REF();
    executor.registers[x_reg] = executor.heap.back();
    executor.registers[a_reg] = executor.heap.back();
}

void wam::put_value(wam::executor &executor, size_t x_reg, size_t a_reg) {
    executor.registers[a_reg] = executor.registers[x_reg];
}

void wam::get_variable(wam::executor &executor, size_t x_reg, size_t a_reg) {
    executor.registers.at(x_reg) = executor.registers.at(a_reg);
}

void wam::get_value(wam::executor &executor, size_t x_reg, size_t a_reg) {
    unify(executor, executor.registers.at(x_reg).index, executor.registers.at(a_reg).index);
}

void wam::call(wam::executor &executor, const functor_view &functor) {
        bfs_organizer* organizer = executor.get_organizer();
        if(!organizer->has_code_for(functor)){
            executor.fail= true;
            return;
        }

        //Assumes right now only 1 program
        auto programs = organizer->get_code_for(functor);
        for(auto& entry : programs){
            executor.cur_prog_code = entry;
        }
}

void wam::proceed(wam::executor &executor) {
    executor.finished = true;
}
