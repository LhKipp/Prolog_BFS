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
        const auto[head_functor, code] = parse_program_term(line);
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
            continue; //Already handeled from down below
        }

        const term_code *next_term_code = next_exec.instructions.top();
        next_exec.instructions.pop();

        next_exec.registers.resize(next_term_code->expected_register_count.x_a_regs_counts);
        //*2 is a heuristic
        next_exec.heap.reserve(next_term_code->expected_register_count.x_a_regs_counts * 2);

        for (const auto &instruction : next_term_code->instructions) {
            instruction(next_exec);
            //if the executor fails we stop executing
            if (next_exec.fail) {
                break;
            }
        }
        if (!next_exec.fail && next_term_code->is_from_original_query()) {
//            std::cout << "Working on atom number: " << next_exec.solves_atom_number<<std::endl;
            //The heap representation for the query atom "solves_atom_number" has been built now on the heap.
            //The var_reg_subs pointing at x/a_regs can now be transformed to point into the heap
            point_reg_substs_to_heap(next_exec);
        }

        if (!next_exec.fail && next_exec.instructions.empty()) {
            //Executor finished successful
            find_temporary_substitutions(next_exec);
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

    substitutions = std::get<1>(query);
    cur_atom_begin = substitutions.begin();
    //Partition the substitutions in tmp and permanents
    tmp_sub_end= std::partition(substitutions.begin(),substitutions.end(),[](const auto& var_a){
        return !var_a.is_permanent_register;
    });

    //Sort the tmp var_reg_subs according to the atom they are from
    std::sort(cur_atom_begin, tmp_sub_end, [&](const auto& sub_a, const auto& sub_b){
        return sub_a.from_atom_number < sub_b.from_atom_number;
    });

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

void wam::bfs_organizer::find_temporary_substitutions(wam::executor &executor){
    std::transform(substitutions.begin(), tmp_sub_end, std::back_inserter(executor.found_substitutions),
                   [&](const auto &var_reg_sub) {
                        //At this point the var_reg_sub point to heap cells
                       return var_substitution{var_reg_sub.var_name,
                                               string_representation_of(executor.heap, var_reg_sub.register_index, functors)};
                   });

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

void wam::bfs_organizer::point_reg_substs_to_heap(const wam::executor &executor) {
    const auto end_of_atom_subs = std::find_if(cur_atom_begin, tmp_sub_end,
                                               [&](const auto &sub) { return sub.from_atom_number > executor.solves_atom_number; });
    for_each(cur_atom_begin, end_of_atom_subs, [&](var_reg_substitution& reg_sub){
      reg_sub.register_index = executor.registers[reg_sub.register_index].index;
    });
    cur_atom_begin = end_of_atom_subs;
}





































