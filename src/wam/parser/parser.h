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
#include "../data/functor_view.h"
#include "util/seen_register.h"

namespace wam {


    /*
     * Parses a query term e.G. p(Z,h(Z,W),f(W))
     */
    std::tuple<std::vector<term_code>, std::vector<var_reg_substitution>>
    parse_query(const std::string &line);
    std::pair<functor_view, std::vector<term_code>> parse_program_term(const std::string &line);


    helper::reg_func_counts assign_registers(node &functor, node* first_body_atom = nullptr);
    node build_tree(const std::string &line);
    std::vector<const node *> flatten(const node &outer_functor);

    template<typename OutputIter>
    void
    to_query_instructions(const std::vector<const node *> &flattened_term, const node &outer_functor, OutputIter out,
                          std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                          bool from_original_query=false);

    template<typename OutputIter>
    void to_program_instructions(const std::vector<const node *> &flattened_term, OutputIter out,
                                 std::unordered_map<wam::helper::seen_register,bool> &seen_regs);

    functor_view make_functor_view(const node &node);

    template<typename OutputIter>
    void find_substitutions(const node &atom, size_t atom_number, OutputIter out);

    std::vector<const node *> flatten_query(const node &node);

    wam::helper::reg_func_counts assign_permanent_registers(const node &program_node, bool program_term);

}


#endif //PROLOG_BFS_PARSER_H
