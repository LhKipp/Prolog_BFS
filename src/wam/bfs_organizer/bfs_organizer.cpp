//
// Created by leonhard on 29.07.19.
//

#include "bfs_organizer.h"
#include "../visual/substitution_util.h"
#include "../compiler/compiler.h"
#include "util/read_program_code.h"
#include "../compiler/parser/parser.h"
#include "../visual/unification_tree/unification_tree.h"

//#define DEBUG

void wam::bfs_organizer::load_program_from_file(const std::string_view file_path) {
    auto code = read_file(file_path);
    load_term_lines(code);
}

void wam::bfs_organizer::load_program(const std::string_view code) {
    return load_term_lines(code);
}

void wam::bfs_organizer::add_code_to_program_code(const std::unordered_map<wam::functor_view, std::vector<wam::rule>>& code){
    std::for_each(code.cbegin(), code.cend(),[&](const auto& it){
        //it->first is the head_functor

        //if the head_functor is already added, append other rules
        auto head_func = program_code.find(it.first);
        if(head_func != program_code.cend()){
            auto& vec = program_code[head_func->first];
            vec.insert(vec.end(), it.second.cbegin(), it.second.cend());
        }else{
            program_code[it.first] = it.second;
        }
    });

    for(auto& entry : program_code) {
        for (auto &rule : entry.second) {
            rule.set_atoms_par();
        }
    }
}

void wam::bfs_organizer::add_code_to_storage(const std::unordered_map<wam::functor_view, std::vector<wam::rule>>& program_code){
    storage.functor_index_map.reserve(storage.functor_index_map.size() + program_code.size());
    storage.functors.reserve(storage.functors.size() + program_code.size());

    std::for_each(program_code.cbegin(), program_code.cend(),[&](const auto& it){
        //it->first is the head_functor

        //if the head_functor is already added simply return
        if(storage.functor_index_map.find(it.first) != storage.functor_index_map.cend()){
            return;
        }
        storage.functor_index_map[it.first] = storage.functors.size();
        storage.functors.push_back(it.first);
    });
}

void wam::bfs_organizer::load_term_lines(const std::string_view code) {
    auto prg_code = wam::compile_program(code, storage);
    add_code_to_storage(prg_code);
    add_code_to_program_code(prg_code);
}


std::optional<std::vector<wam::var_binding>> wam::bfs_organizer::get_answer() {
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
            instruction(*next_exec);
            //if the executor fails we stop executing
            if (next_exec->failed()) {
                next_exec->clear();
                break;
            }
        }

        if(next_exec->succeeded()){
            return find_substitutions_from_orig_query(*next_exec, storage);
        }
    }

    //no more executor
    return std::nullopt;
}


void wam::bfs_organizer::load_query(const std::string &query_line) {
    //Clear the old executors
    executors.clear();
    //parse the query and save the results
    current_query_code = compile_query(query_line, storage);

    init_executor = executor{current_query_code.atoms().size()};
    //Copy references to query instructions into the executor instructions
    std::transform(current_query_code.atoms().rbegin(), current_query_code.atoms().rend(),
                  init_executor.term_codes.begin(),
                  [&](compiled_atom &term_code) {
                    return &term_code;
                  });
    init_executor.organizer = this;
    executors.push_back(&init_executor);
}

void wam::bfs_organizer::clear(){
    executors.clear();
    storage.functor_index_map.clear();
    storage.functors.clear();
    program_code.clear();
    current_query_code.atoms().clear();
}

wam::parser_error wam::bfs_organizer::validate_program(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    try{
        wam::_program_grammar::result_t parser_result;
        parse_program(code, parser_result);
        return parser_error{};
    }catch(const parser_error& e){
        return e;
    }
}

wam::parser_error wam::bfs_organizer::validate_query(const std::string_view code) {
//TODO the code uses the parser code, a simple syntax checker would be good enough here
    try{
        wam::_query_grammar::result_t query; //= parser result
        parse_query(code, query);
        return parser_error{};
    }catch(const parser_error& e){
        return e;
    }
}

wam::query_node wam::bfs_organizer::get_unification_tree() const{
    return wam::make_tree(init_executor, storage);
}

wam::bfs_organizer::bfs_organizer() {
    auto result = get_build_in_predicates(storage);
    add_code_to_storage(result);
    add_code_to_program_code(result);
}

