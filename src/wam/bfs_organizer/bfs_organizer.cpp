//
// Created by leonhard on 29.07.19.
//

#include <prolog_bfs/wam/bfs_organizer/util/bfs_util.h>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include "prolog_bfs/wam/visual/substitution_util.h"
#include "prolog_bfs/wam/compiler/compiler.h"
#include "prolog_bfs/wam/bfs_organizer/util/read_program_code.h"
#include "prolog_bfs/wam/compiler/parser/parser.h"
#include "prolog_bfs/wam/visual/unification_tree/unification_tree.h"

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
        return wam::result(wam::runtime_error{ERROR_TYPE::OUT_OF_MEMORY, "Memory exhausted."}, runtime_stats);
    }
}

wam::result wam::bfs_organizer::exec_executors(){
    using namespace std::chrono;
    system_clock::time_point start_time = system_clock::now();

    while (!executors.empty()) {

        //Check that time_limit is not exceeded
        auto elapsed_time = system_clock::now() - start_time;
        if(elapsed_time > this->time_limit){
            return wam::result{wam::runtime_error{ERROR_TYPE::OUT_OF_TIME, "Time limit exceeded"}, runtime_stats};
        }


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
                return wam::result{next_exec->get_runtime_err(), runtime_stats};
            }
            //if the executor fails we stop executing
            if (next_exec->failed()) {
                next_exec->clear();
                break;
            }
        }

        if(next_exec->succeeded()){
            return wam::result{find_substitutions_from_orig_query(*next_exec, storage), runtime_stats};
        }
    }

    //no more executor
    return result{std::nullopt, runtime_stats};
}


compiler::error wam::bfs_organizer::load_query(const std::string &query_line) {
    auto query = append_dot_if_not_present(query_line);
    //Clear the old executors
    executors.clear();
    //Clear the old statistics
    runtime_stats = wam::runtime_statistics{};
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
    //Clearing in a tree, calls all destructor in dfs order (?).
    //That takes a lot of memory depending on the depth
    //So we clear all objects which in the destructor do not call that many other
    //destructors first.
    //clear other global storage
    storage.clear_memory();
    std::list<executor*>().swap(executors);

    std::vector<compiled_atom>().swap(current_query_code.atoms());
    std::unordered_map<functor_view, std::vector<rule>>().swap(program_code);
    std::unordered_map<functor_view, std::vector<rule>>().swap(built_in_preds);

    //Clear all the executors
    std::vector<std::unique_ptr<executor>>().swap(init_executor.children);
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
    try{
        return wam::make_tree(init_executor, storage);
    }catch(const std::bad_alloc& e){
        auto err_node = wam::query_node{nullptr, 0, 0};
        err_node.set_resolved_name("Not enough memory to generate the tree");
        return err_node;
    }
}

wam::bfs_organizer::bfs_organizer() : built_in_preds{get_build_in_predicates(storage)}{
    program_code = built_in_preds;
}

void bfs_organizer::set_time_limit(const std::chrono::microseconds &timeLimit) {
    this->time_limit = timeLimit;
}

