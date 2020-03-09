//
// Created by leonhard on 29.07.19.
//

#include "bfs_organizer.h"
#include "../visual/substitution_util.h"
#include "../compiler/compiler.h"
#include "util/read_program_code.h"
#include "../compiler/parser/parser.h"
#include "../visual/unification_tree/unification_tree.h"

void wam::bfs_organizer::load_program_from_file(const std::string_view file_path) {
    auto code = read_file(file_path);
    load_term_lines(code);
}

void wam::bfs_organizer::load_program(const std::string_view code) {
    return load_term_lines(code);
}

void wam::bfs_organizer::load_term_lines(const std::string_view code) {
    program_code = wam::compile_program(code);

    functor_index_map.reserve(program_code.size());
    functors.reserve(program_code.size());

    std::for_each(program_code.cbegin(), program_code.cend(),[&](const auto& it){
        //it->first is the head_functor

        //if the head_functor is already added simply return
        if(functor_index_map.find(it.first) != functor_index_map.cend()){
           return;
        }
        functor_index_map[it.first] = functors.size();
        functors.push_back(it.first);
    });
}


std::optional<std::vector<wam::var_binding>> wam::bfs_organizer::get_answer() {
    while (!executors.empty()) {
        executor* next_exec = executors.front();
        executors.pop_front();


        term_code *next_term_code = next_exec->term_codes.back();

        next_exec->registers.resize(next_term_code->expected_register_count);
        //*2 is a heuristic
        next_exec->heap.reserve(next_term_code->expected_register_count * 2);

        for (const auto &instruction : next_term_code->instructions) {
            instruction(*next_exec);
            //if the executor fails we stop executing
            if (next_exec->failed()) {
                break;
            }
        }

        if(next_exec->succeeded()){
            return find_substitutions_from_orig_query(*next_exec, functors);
        }
    }

    //no more executor
    return std::nullopt;
}


void wam::bfs_organizer::load_query(const std::string &query_line) {
    //Clear the old executors
    executors.clear();
    //parse the query and save the results
    current_query_code = compile_query(query_line);

    init_executor = executor{current_query_code.size()};
    //Copy references to query instructions into the executor instructions
    std::transform(current_query_code.rbegin(), current_query_code.rend(),
                  init_executor.term_codes.begin(),
                  [&](term_code &term_code) {
                    return &term_code;
                  });
    init_executor.organizer = this;
    executors.push_back(&init_executor);
}

void wam::bfs_organizer::clear(){
    executors.clear();
    functor_index_map.clear();
    functors.clear();
    program_code.clear();
    current_query_code.clear();
}

wam::parser_error wam::bfs_organizer::validate_program(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    try{
        std::vector<boost::optional<node>> parser_result;
        parse_program(code, parser_result);
        return parser_error{};
    }catch(const parser_error& e){
        return e;
    }
}

wam::parser_error wam::bfs_organizer::validate_query(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    try{
        std::vector<node> query_nodes; //= compiler result
        parse_query(code, query_nodes);
        return parser_error{};
    }catch(const parser_error& e){
        return e;
    }
}

wam::query_node wam::bfs_organizer::get_unification_tree() const{
    return wam::make_tree(init_executor, functors);
}
