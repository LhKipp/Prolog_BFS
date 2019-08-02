//
// Created by leonhard on 29.07.19.
//

#include "bfs_organizer.h"
#include "util/substitution_util.h"
#include "../parser/parser.h"


void wam::bfs_organizer::load_program(const std::vector<std::string> &lines) {
    program_code.reserve(lines.size());
    for(auto& line : lines){
        term_code code = parse_program_term(line);
        program_code.emplace(code.term_functor_name, code);
    }
}


std::tuple<std::vector<wam::var_substitution>, bool> wam::bfs_organizer::get_answer() {
    while(!executors.empty()){
        executor next_exec = executors.front();
        executors.pop();

        next_exec.registers.resize(next_exec.cur_prog_code->expected_register_count.x_a_regs_counts);
        for(auto& instruction : next_exec.cur_prog_code->instructions){
            instruction(next_exec);
            //if the executor fails we stop executing
            if(next_exec.fail){
                break;
            }
        }

        if(next_exec.fail){
            continue;
        }

        return std::make_tuple(substitutions_for(substitutions,next_exec), true);
    }

    //no more executor
    return std::make_tuple<std::vector<var_substitution>, bool>({}, false);
}


void wam::bfs_organizer::load_query(const std::string &query_line) {
    //parse the query and save the results
    auto result = parse_query(query_line);
    auto q = std::get<0>(result);
    auto query = q[0];
    //Right now the substitutions point to some x_regs
    substitutions = std::get<1>(result);
    //Increase the size of the functor vector
    functors.reserve(query.expected_register_count.func_count);
    functor_index_map.reserve(query.expected_register_count.func_count);

    //create one initialiser executor
    executor init_executor;
    init_executor.organizer = this;
    init_executor.registers.resize(query.expected_register_count.x_a_regs_counts);
    init_executor.heap.reserve(query.expected_register_count.x_a_regs_counts * 2);
    for (auto &instruction : query.instructions) {
        instruction(init_executor);
    }
    if(init_executor.fail){//If the call instruction failed --> return
        return;
    }else{
        executors.push(init_executor);
    }

    //Now we let the substitutions point into the heap
    for (auto &substitution : substitutions) {
        substitution.register_index = init_executor.registers[substitution.register_index].index;
    }

}

bool wam::bfs_organizer::has_code_for(const functor_view& functor)const {
    return program_code.find(functor) != program_code.end();
}

std::vector<wam::term_code *>wam::bfs_organizer::get_code_for(const functor_view &functor) {
    auto programs_range = program_code.equal_range(functor);

    std::vector<wam::term_code*> result;
    std::for_each(programs_range.first, programs_range.second,
            [&](auto &entry){
        result.push_back(&entry.second);
    });
    return result;
}

