//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_BFS_ORGANIZER_H
#define PROLOG_BFS_BFS_ORGANIZER_H


#include <unordered_map>
#include <vector>
#include <list>
#include <experimental/filesystem>
#include "../data/functor_view.h"
#include "../data/var_substitution.h"
#include "../data/term_code.h"
#include "../data/var_reg_substitution.h"
#include "../executor/executor.h"
#include "../instructions/instructions.h"
#include "../compiler/parser/parser_error.h"


namespace wam {

    class bfs_organizer {
        friend struct executor;
        friend void wam::call(wam::executor &old_executor, const functor_view &functor);
        friend void wam::proceed(wam::executor &old_exec);
        friend void wam::deallocate(wam::executor &executor);
    private:
        //Queue of executors, to execute
        std::list<executor> executors;

        //Global storage for all executors
        std::unordered_map<functor_view, size_t> functor_index_map;
        std::vector<functor_view> functors;
        std::unordered_multimap<functor_view, std::vector<term_code>> program_code;
        std::vector<term_code> current_query_code;

        std::vector<var_reg_substitution> permanent_substitutions;


        std::vector<executor> leaf_execs;
        void archive_finished_exec(executor&& executor){
            //To generate the unification tree out of all executors, we need the
            //executors who finished (fail or success) because
            executor.clear();
            leaf_execs.push_back(std::move(executor));
        }

        //Executors, finished execution with call or proceed, saved so heap is still accessible
        std::vector<executor> node_executors;
        /**
         * Archives the given executor
         * @param executor the executor to store
         */
        void inline archive_node_exec(executor&& executor){
            node_executors.push_back(std::move(executor));
        }

        inline const executor& get_archived(size_t index){
            return node_executors.at(index);
        }

        size_t inline next_archive_index(){
            return node_executors.size();
        }

        inline executor& parent_of(const executor& exec){
            assert(exec.has_parent());
            assert(exec.parent_index < 100000);
            return node_executors.at(exec.parent_index);
        }

        std::vector<wam::var_substitution> find_substitutions(const executor &executor);

        void load_term_lines(std::string_view term_lines);
    public:
        void clear();

        wam::parser_error validate_program(std::string_view code);
        wam::parser_error validate_query(std::string_view code);
        void load_program(std::string_view code);
        void load_program_from_file(std::string_view file_path);


        void load_query(const std::string &query);


        /*
         * Returns var_substitutions if found otherwise std::nullopt
         * Note: This may run into an endless loop
         */
        std::optional<std::vector<wam::var_substitution>> get_answer();

        bool has_code_for(const functor_view &functor) const{
            return program_code.find(functor) != program_code.end();
        }

    };


}


#endif //PROLOG_BFS_BFS_ORGANIZER_H
