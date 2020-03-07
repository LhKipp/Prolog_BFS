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
#include "../data/var_binding.h"
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

        executor init_executor;

        //Queue of executors, to execute
        std::list<executor*> executors;

        //Global storage for all executors
        std::unordered_map<functor_view, size_t> functor_index_map;
        std::vector<functor_view> functors;
        std::unordered_multimap<functor_view, std::vector<term_code>> program_code;
        std::vector<term_code> current_query_code;

        //std::vector<var_reg_substitution> permanent_substitutions;

        //Executors, finished execution with call or proceed, saved so heap is still accessible
        std::vector<wam::var_binding> find_substitutions(const executor &executor);

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
        std::optional<std::vector<wam::var_binding>> get_answer();

        bool has_code_for(const functor_view &functor) const{
            return program_code.find(functor) != program_code.end();
        }

    };


}


#endif //PROLOG_BFS_BFS_ORGANIZER_H
