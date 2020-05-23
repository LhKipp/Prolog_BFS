//
// Created by leonhard on 29.07.19.
//

#include <wam/bfs_organizer/util/bfs_util.h>
#include "bfs_organizer.h"
#include "../visual/substitution_util.h"
#include "../compiler/compiler.h"
#include "util/read_program_code.h"
#include "../compiler/parser/parser.h"
#include "../visual/unification_tree/unification_tree.h"

//#define DEBUG

compiler::error wam::bfs_organizer::load_program_from_file(const std::string_view file_path) {
    auto code = read_file(file_path);
    return load_term_lines(code);
}

compiler::error wam::bfs_organizer::load_program(const std::string_view code) {
    return load_term_lines(code);
}

void wam::bfs_organizer::merge_program_and_built_in_preds(){
    std::for_each(built_in_preds.cbegin(), built_in_preds.cend(),[&](const auto& it){
        program_code[it.first] = it.second;
    });

    for(auto& entry : program_code) {
        for (auto &rule : entry.second) {
            rule.set_atoms_par();
        }
    }
}


compiler::error wam::bfs_organizer::load_term_lines(const std::string_view code) {
    try{
        program_code = wam::compile_program(code, storage);
    }catch(compiler::error& err){
        return err;
    }
    merge_program_and_built_in_preds();

    //everything worked. return empty error
    return compiler::error{};
}


wam::result wam::bfs_organizer::get_answer() {
    try{
        return exec_executors();
    }catch(const std::bad_alloc& err){
        clear_memory();
        return wam::result(wam::runtime_error{ERROR_TYPE::OUT_OF_MEMORY, "Memory exhausted."});
    }
}

wam::result wam::bfs_organizer::exec_executors(){
    while (!executors.empty()) {
        executor* next_exec = executors.front();
        executors.pop_front();

        compiled_atom *next_term_code = next_exec->term_codes.back();
#ifdef DEBUG
        std::cout << "executing: " << next_term_code->get_code_info().value << " From line: "
        << next_term_code->get_code_info().line;
#endif

        next_exec->registers.resize(next_term_code->expected_register_count);
        //*2 is a heuristic
        next_exec->heap.reserve(next_term_code->expected_register_count * 2);

        for (const auto &instruction : next_term_code->instructions) {
            try{
                instruction(*next_exec);
            }catch(ERROR_TYPE runtime_err){
                next_exec->clear();
                return wam::result{next_exec->get_runtime_err()};
            }
            //if the executor fails we stop executing
            if (next_exec->failed()) {
                next_exec->clear();
                break;
            }
        }

        if(next_exec->succeeded()){
            return wam::result{find_substitutions_from_orig_query(*next_exec, storage)};
        }
    }

    //no more executor
    return result{std::nullopt};
}


compiler::error wam::bfs_organizer::load_query(const std::string &query_line) {
    auto query = append_dot_if_not_present(query_line);
    //Clear the old executors
    executors.clear();
    //parse the query and save the results
    try{
        current_query_code = compile_query(query, storage);
    }catch(compiler::error& err){
        return err;
    }

    init_executor = executor{current_query_code.atoms().size()};
    //Copy references to query instructions into the executor instructions
    std::transform(current_query_code.atoms().rbegin(), current_query_code.atoms().rend(),
                  init_executor.term_codes.begin(),
                  [&](compiled_atom &term_code) {
                    return &term_code;
                  });
    init_executor.organizer = this;
    executors.push_back(&init_executor);

    //everything worked. return empty err
    return compiler::error{};
}

void wam::bfs_organizer::clear_memory(){
    //Clear all the executors
    std::vector<std::unique_ptr<executor>>().swap(init_executor.children);
    //Remove them from execution list
    executors.clear();

    //clear other global storage
    storage.clear_memory();
    std::unordered_map<functor_view, std::vector<rule>>().swap(program_code);
    std::unordered_map<functor_view, std::vector<rule>>().swap(built_in_preds);
    std::vector<compiled_atom>().swap(current_query_code.atoms());
}

compiler::error wam::bfs_organizer::validate_program(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    try{
        wam::_program_grammar::result_t parser_result;
        parse_program(code, parser_result);
        return compiler::error{};
    }catch(const compiler::error& e){
        return e;
    }
}

compiler::error wam::bfs_organizer::validate_query(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    auto query_code = append_dot_if_not_present(std::string(code));
    try{
        wam::_query_grammar::result_t query; //= parser result
        parse_query(query_code, query);
        return compiler::error{};
    }catch(const compiler::error& e){
        return e;
    }
}

wam::query_node wam::bfs_organizer::get_unification_tree() const{
    return wam::make_tree(init_executor, storage);
}

wam::bfs_organizer::bfs_organizer() : built_in_preds{get_build_in_predicates(storage)}{
    program_code = built_in_preds;
}

