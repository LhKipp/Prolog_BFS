//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_BFS_ORGANIZER_H
#define PROLOG_BFS_BFS_ORGANIZER_H


#include <unordered_map>
#include <vector>
#include <list>
#include "../data/functor_view.h"
#include "../data/var_substitution.h"
#include "../data/term_code.h"
#include "../data/var_reg_substitution.h"
#include "../executor/executor.h"
#include "../instructions/instructions.h"

namespace wam {

    using var_substitutions = std::vector<wam::var_substitution>;
    class bfs_organizer {
        friend struct executor;
        friend void wam::call(wam::executor &old_executor, const functor_view &functor, bool from_original_query);
        friend void wam::proceed(wam::executor &executor);
        friend void wam::deallocate(wam::executor &executor);
    private:
        //Memory usage of all executors
        size_t memory_usage = 0;
        //Queue of executors
        std::list<executor> executors;
        //Global storage for all executors
        std::unordered_map<functor_view, size_t> functor_index_map;
        std::vector<functor_view> functors;

        std::unordered_multimap<functor_view, std::vector<term_code>> program_code;
        std::vector<term_code> current_query_code;

        std::vector<var_reg_substitution> permanent_substitutions;
        void find_temporary_substitutions(executor&);
        void find_permanent_substitutions(executor &executor);
        void load_term_lines(const std::vector<std::string>& term_lines);


    public:
        void clear();

        void load_program(const std::vector<std::string> &lines);
        void load_program(const std::string &file_path);

        void load_query(const std::string &query_line);


        /*
         * Returns a bool indicating whether an answer has been found (true = has answer) and a var_substitution.
         * Note: This may run into an endless loop
         */
        std::optional<std::vector<wam::var_substitution>> get_answer();

        bool has_code_for(const functor_view &functor) const;

        void point_reg_substs_to_heap(executor &executor);
    };


}


#endif //PROLOG_BFS_BFS_ORGANIZER_H
