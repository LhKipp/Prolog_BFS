//
// Created by leonhard on 29.07.19.
//

#include "bfs_organizer.h"
#include <numeric>
#include <iostream>
#include "util/substitution_util.h"
#include "../parser/parser.h"
#include <iterator>
#include "util/read_program_code.h"

void wam::bfs_organizer::load_program(const std::string &file_path) {
    load_term_lines(read_program_code(file_path));
}

void wam::bfs_organizer::load_program(const std::vector<std::string> &program_lines) {
    load_term_lines(read_program_code(program_lines));
}

void wam::bfs_organizer::load_term_lines(const std::vector<std::string> &term_lines) {
    program_code.reserve(term_lines.size());
    //Assumes at max there is a different head at every line
    functor_index_map.reserve(term_lines.size());
    functors.reserve(term_lines.size());
    //TODO Change heuristic into configuration
    //The more lines the program has, the more complex --> the more depth
    dead_executors.reserve(term_lines.size());

    for (auto &line : term_lines) {
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
        executors.pop_front();
        // std::cout << memory_usage << std::endl;
        // if(memory_usage >  1000 * 1000 * 1000){
        //   executors.clear();
        //   return std::nullopt;
        // }
        if (next_exec.instructions.empty()) {
            find_temporary_substitutions(next_exec);
            find_permanent_substitutions(next_exec);
            return std::optional<std::vector<wam::var_substitution>>{next_exec.found_substitutions};
        }

        const term_code *next_term_code = next_exec.instructions.top();
        next_exec.instructions.pop();

        next_exec.registers.resize(next_term_code->expected_register_count.x_a_regs_counts);
        //*2 is a heuristic
        next_exec.heap.reserve(next_term_code->expected_register_count.x_a_regs_counts * 2);

        const auto size_before = executors.size();


        for (const auto &instruction : next_term_code->instructions) {
            instruction(next_exec);
            //if the executor fails we stop executing
            if (next_exec.fail) {
                break;
            }
        }
        if (!next_exec.fail && next_term_code->is_from_original_query()) {
            //TODO Debug
//            std::cout << "Working on atom number: " << next_exec.solves_atom_number << std::endl;
            //The heap representation for the query atom "solves_atom_number" has been built now on the heap.
            //The var_reg_subs pointing at x/a_regs can now be transformed to point into the heap
            auto elem = executors.rbegin();
            const auto new_elem_count = executors.size()- size_before;
            for (int i = 0; i < new_elem_count; ++i, ++elem) {
                point_reg_substs_to_heap(*elem);
            }
        }
    }

    //no more executor
    return std::nullopt;
}


void wam::bfs_organizer::load_query(const std::string &query_line) {
    //Clear the old executors
    executors.clear();
    //parse the query and save the results
    auto query = parse_query(query_line);
    current_query_code = std::get<0>(query);

    std::vector<var_reg_substitution> all_var_reg_substitutions = std::get<1>(query);
    //Partition the substitutions in tmp and permanents
    const auto tmp_sub_end = std::partition(all_var_reg_substitutions.begin(), all_var_reg_substitutions.end(),
                                            [](const auto &var_a) {
                                                return !var_a.is_permanent_register;
                                            });
    //Copy the permanent registers
    permanent_substitutions.assign(tmp_sub_end, all_var_reg_substitutions.end());

    //Sort the tmp var_reg_subs according to the atom they are from
    std::sort(all_var_reg_substitutions.begin(), tmp_sub_end, [&](const auto &sub_a, const auto &sub_b) {
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
    init_executor.substitutions.assign(all_var_reg_substitutions.begin(), tmp_sub_end);
    init_executor.cur_atom_begin = 0;

    executors.push_back(init_executor);
}

bool wam::bfs_organizer::has_code_for(const functor_view &functor) const {
    return program_code.find(functor) != program_code.end();
}

void wam::bfs_organizer::find_temporary_substitutions(wam::executor &executor) {
    std::transform(executor.substitutions.begin(), executor.substitutions.end(),
                   std::back_inserter(executor.found_substitutions),
                   [&](const auto &var_reg_sub) {
                       //At this point the var_reg_sub point to heap cells
                       return var_substitution{var_reg_sub.var_name,
                                               string_representation_of(executor, var_reg_sub.register_index,
                                                                        functors)};
                   });

}

void wam::bfs_organizer::find_permanent_substitutions(wam::executor &executor) {
    std::transform(permanent_substitutions.begin(), permanent_substitutions.end(),
                   std::back_inserter(executor.found_substitutions),
                   [&](const auto &var_reg_sub) {
                       //The y_register indexes point to register cells, but string representation searches in the heap
                       //Take the index from the register first
                       const auto index = executor.environments.top().
                               permanent_registers[var_reg_sub.register_index].index;
                       return var_substitution{var_reg_sub.var_name,
                                               string_representation_of(executor, index, functors)};
                   });
}

void wam::bfs_organizer::point_reg_substs_to_heap(executor &executor) {
    const auto end_of_atom_subs = std::find_if(executor.substitutions.begin() + executor.cur_atom_begin,
                                               executor.substitutions.end(),
                                               [&](const auto &sub) {
                                                   return sub.from_atom_number > executor.solves_atom_number;
                                               });
    for_each(executor.substitutions.begin() + executor.cur_atom_begin,
             end_of_atom_subs,
             [&](var_reg_substitution &reg_sub) {
                 reg_sub.register_index = executor.registers[reg_sub.register_index].index;
             });
//    std::cout << "cur_atom_begin was: " << executor.cur_atom_begin<< std::endl;
    executor.cur_atom_begin = end_of_atom_subs - executor.substitutions.begin();
//    std::cout << "cur_atom_begin is now: " << executor.cur_atom_begin<< std::endl;

}



void wam::bfs_organizer::clear(){
    executors.clear();
    functor_index_map.clear();
    functors.clear();
    program_code.clear();
    current_query_code.clear();
    permanent_substitutions.clear();
}

size_t wam::bfs_organizer::archive(const wam::executor &executor) {
    dead_executors.push_back(executor);
    return dead_executors.size() -1;
}




































