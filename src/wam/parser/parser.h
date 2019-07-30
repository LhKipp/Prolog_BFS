//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_PARSER_H
#define PROLOG_BFS_PARSER_H


#include <string>
#include <functional>
#include "util/node.h"
#include "../executor/executor.h"
#include "util/reg_func_counts.h"
#include "../data/term_code.h"

namespace wam {


    /*
     * Parses a query term e.G. p(Z,h(Z,W),f(W))
     */
    std::tuple<term_code, std::vector<var_reg_substitution>> parse_query(const std::string &line);
    term_code parse_program_term(const std::string &line);


    helper::reg_func_counts assign_registers(node &top_node);
    node build_tree(const std::string &line);
    std::vector<const node *> flatten( const node &top_node);

    std::vector<std::function<void(executor & )>> to_query_instructions(const std::vector<const node *>& flattened_term);
    std::vector<std::function<void(executor & )>> to_program_instructions(const std::vector<const node *>& flattened_term);

    functor_view make_functor_view(const node &node);

    std::vector<var_reg_substitution> find_substitutions(const node &top_node);
}


#endif //PROLOG_BFS_PARSER_H
