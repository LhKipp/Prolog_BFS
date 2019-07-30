//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_BFS_ORGANIZER_H
#define PROLOG_BFS_BFS_ORGANIZER_H


#include "../data/functor_view.h"
#include "../data/term_code.h"
#include "../parser/parser.h"
#include "../data/var_substitution.h"
#include <unordered_map>
#include <vector>
#include <queue>

namespace wam {
    using var_substitutions = std::vector<wam::var_substitution>;

    class bfs_organizer {
    private:
        //Global storage for all executors
        std::unordered_map<functor_view, size_t> functor_index_map;
        std::vector<functor_view> functors;

        std::unordered_multimap<functor_view, term_code> program_code;

        std::vector<var_reg_substitution> substitutions;

        std::queue<executor> executors;

    public:
        void load_program(const std::vector<std::string> &lines);

        void load_query(const std::string &query_line);


        /*
         * Returns a bool indicating whether an answer has been found (true = has answer) and a var_substitution.
         * Note: This may run into an endless loop
         */
        std::tuple<var_substitutions, bool> get_answer();

    };
}


#endif //PROLOG_BFS_BFS_ORGANIZER_H
