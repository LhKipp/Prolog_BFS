//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_BFS_ORGANIZER_H
#define PROLOG_BFS_BFS_ORGANIZER_H


#include <unordered_map>
#include <vector>
#include <list>
#include <chrono>
#include "../data/functor_view.h"
#include "../data/var_binding.h"
#include "../data/compiled_atom.h"
#include "../data/var_reg_substitution.h"
#include "../executor/executor.h"
#include "../instructions/instructions.h"
#include "../visual/unification_tree/query_node.h"
#include "../data/rule.h"
#include "data/storage.h"
#include <prolog_bfs/wam/bfs_organizer/data/result.h>
#include <prolog_bfs/wam/compiler/error/compiler_error.h>
#include <prolog_bfs/wam/bfs_organizer/data/runtime_statistics.h>


namespace wam {

    class bfs_organizer {
        friend struct executor;
        friend void wam::call(wam::executor &old_executor, const functor_view &functor);
        friend void wam::proceed(wam::executor &old_exec);
        friend void wam::deallocate(wam::executor &executor);
    private:

        executor init_executor;
        wam::runtime_statistics runtime_stats;

        //Queue of executors, to execute
        std::list<executor*> executors;

        std::chrono::microseconds time_limit{100'000};

        wam::storage storage;
        //functor to multiple term_codes
        std::unordered_map<functor_view, std::vector<rule>> program_code;
        std::unordered_map<functor_view, std::vector<rule>> built_in_preds;
        wam::rule current_query_code;

        compiler::error load_term_lines(std::string_view term_lines);
        wam::result exec_executors();
    public:
        void set_time_limit(const std::chrono::microseconds &timeLimit);

        bfs_organizer();
        void clear_memory();

        compiler::error validate_program(std::string_view code);
        compiler::error validate_query(std::string_view code);

        compiler::error load_program(std::string_view code);
        compiler::error load_program_from_file(std::string_view file_path);

        compiler::error load_query(const std::string &query);

        query_node get_unification_tree() const;


        /*
         * Returns var_substitutions if found otherwise std::nullopt
         * Note: This may run into an endless loop
         */
        wam::result get_answer();

        bool has_code_for(const functor_view &functor) const{
            return program_code.find(functor) != program_code.end();
        }

        void merge_program_and_built_in_preds();

    };


}


#endif //PROLOG_BFS_BFS_ORGANIZER_H
