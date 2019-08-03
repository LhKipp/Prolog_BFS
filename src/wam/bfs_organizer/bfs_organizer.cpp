//
// Created by leonhard on 29.07.19.
//

#include "bfs_organizer.h"
#include <numeric>
#include <iostream>
#include "util/substitution_util.h"
#include "../parser/parser.h"
#include <iterator>
//#include "../../util/vector_util.h"

void wam::bfs_organizer::load_program(const std::vector<std::string> &lines) {
    program_code.reserve(lines.size());
    //Assumes at max there is a different head at every line
    functor_index_map.reserve(lines.size());
    functors.reserve(lines.size());
    for (auto &line : lines) {
        const auto [head_functor, code] = parse_program_term(line);
        program_code.emplace(head_functor, code);

        //This is add_if_not_present
        //If this head_functor has not been added to functors yet
        if (functor_index_map.find(head_functor) == functor_index_map.end()) {
            functor_index_map[head_functor] = functors.size();
            functors.push_back(head_functor);
        }
    }
}


std::optional<std::vector<wam::var_substitution>> wam::bfs_organizer::get_answer() {
    while (!executors.empty()) {
        executor next_exec = executors.front();
        executors.pop();

        if (next_exec.instructions.empty()) {
            continue; //Executor has been processed already
        }
        const term_code *next_term_code = next_exec.instructions.top();
        next_exec.instructions.pop();

        next_exec.registers.resize(next_term_code->expected_register_count.x_a_regs_counts);
        //*2 is a heuristic
        next_exec.heap.reserve(next_term_code->expected_register_count.x_a_regs_counts * 2);

        //Check for x_reg / temporary substitutions to find in the heap
        //If we encounter term code for atom 3, we know temporariers for atom 2 have been built in the heap
        //Thats why for atom number 0 we skip here
        if (next_term_code->is_from_original_query()){
            if(next_exec.solves_atom_number){
                find_temporary_substitutions(next_exec, next_exec.solves_atom_number -1);
            }
            ++next_exec.solves_atom_number;
        }

        for (const auto &instruction : next_term_code->instructions) {
            instruction(next_exec);
            //if the executor fails we stop executing
            if (next_exec.fail) {
                break;
            }
        }

        if (next_exec.fail) {
            continue;
        }

        if (next_exec.instructions.empty()) {
            //Executor finished successful
            find_temporary_substitutions(next_exec, next_exec.solves_atom_number);
            find_permanent_substitutions(next_exec);
            return std::optional<std::vector<wam::var_substitution>>{next_exec.found_substitutions};
        }
    }

    //no more executor
    return std::nullopt;
}


void wam::bfs_organizer::load_query(const std::string &query_line) {
    //parse the query and save the results
    auto query = parse_query(query_line);
    current_query_code = std::get<0>(query);

    //Right now the substitutions point to some x_regs or y_regs
    //The method find_temporary_substitutions will let the x_regs point to heap regs
    //The method find_permanent_substitutions will let the y_regs point to heap regs
    substitutions = std::get<1>(query);
    cur_atom_begin = substitutions.begin();
    tmp_sub_end = std::partition_point(substitutions.begin(),
                                                  substitutions.end(),
                                                  [](const auto &sub) { return !sub.is_permanent_register; });

    //debug
    assert(tmp_sub_end == substitutions.end());

    //create one initialiser executor
    executor init_executor;
    //Copy references to query instructions into the executor instructions
    std::for_each(current_query_code.rbegin(), current_query_code.rend(),
                  [&](const term_code &term_code) {
                      init_executor.instructions.push(&term_code);
                  });
    init_executor.organizer = this;

    executors.push(init_executor);
}

bool wam::bfs_organizer::has_code_for(const functor_view &functor) const {
    return program_code.find(functor) != program_code.end();
}

//std::vector<wam::term_code *> wam::bfs_organizer::get_code_for(const functor_view &functor) {
//    auto programs_range = program_code.equal_range(functor);
//
//    std::vector<wam::term_code *> result;
//    std::for_each(programs_range.first, programs_range.second,
//                  [&](auto &entry) {
//                      result.push_back(&entry.second);
//                  });
//    return result;
//}

/*
 * Finds all temporary substitutions in executor
 *
 */
void wam::bfs_organizer::find_temporary_substitutions(wam::executor &executor, size_t atom_number) {
    for(const auto& var_sub: executor.found_substitutions){
        std::cout << var_sub;
    }
    const auto end_of_atom_subs = std::find_if(cur_atom_begin, tmp_sub_end,
                                               [=](const auto &sub) { return sub.from_atom_number > atom_number; });
    std::transform(cur_atom_begin, end_of_atom_subs, std::back_inserter(executor.found_substitutions),
                   [&](const auto &var_reg_sub) {
                       //The register indexes point to register cells, but string representation searches in the heap
                       //Take the index from the register first
                       const auto index = executor.registers[var_reg_sub.register_index].index;
                       return var_substitution{var_reg_sub.var_name,
                                               string_representation_of(executor.heap, index, functors)};
                   });

    cur_atom_begin = end_of_atom_subs;
    for(const auto& var_sub: executor.found_substitutions){
        std::cout << var_sub;
    }
    std::cout.flush();
}

void wam::bfs_organizer::find_permanent_substitutions(wam::executor &executor) {
    std::transform(tmp_sub_end, substitutions.end(), std::back_inserter(executor.found_substitutions),
                   [&](const auto &var_reg_sub) {
                       //The y_register indexes point to register cells, but string representation searches in the heap
                       //Take the index from the register first
                       const auto index = executor.environments.top().
                               permanent_registers[var_reg_sub.register_index].index;
                       return var_substitution{var_reg_sub.var_name,
                                               string_representation_of(executor.heap, index, functors)};
                   });
}





































