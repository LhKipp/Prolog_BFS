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
    std::tuple<std::vector<term_code>, std::vector<var_reg_substitution>> parse_query(const std::string &line);
    std::vector<term_code> parse_program_term(const std::string &line);


    helper::reg_func_counts assign_registers(node &functor);
    node build_tree(const std::string &line);
    std::vector<const node *> flatten(const node &outer_functor);

    std::vector<std::function<void(executor & )>> to_query_instructions(const std::vector<const node *>& flattened_term, const node& outer_functor);
    std::vector<std::function<void(executor & )>> to_program_instructions(const std::vector<const node *>& flattened_term);

    functor_view make_functor_view(const node &node);

    std::vector<var_reg_substitution> find_substitutions(const node &top_node);

    std::vector<const node *> flatten_query(const node &node);

    size_t assign_permanent_registers(const node &program_node, bool program_term);

}


#endif //PROLOG_BFS_PARSER_H
