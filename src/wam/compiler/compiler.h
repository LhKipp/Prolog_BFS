//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_COMPILER_H
#define PROLOG_BFS_COMPILER_H


#include <string>
#include <functional>
#include "util/node.h"
#include "../executor/executor.h"
#include "../data/compiled_atom.h"
#include "../data/functor_view.h"
#include "util/seen_register.h"
#include "../data/rule.h"
#include "../bfs_organizer/data/storage.h"

namespace wam {

    std::unordered_map<wam::functor_view, std::vector<wam::rule>>
    get_build_in_predicates(storage& storage);

    std::pair<functor_view, wam::rule> compile_program_term(std::vector<node>& atoms, storage& storage, bool compilation_of_built_in_preds = false);

    int assign_registers(node &functor, node* first_body_atom = nullptr);
    int assign_permanent_registers(std::vector<node> &nodes, bool program_term);

    std::vector<const node *> flatten_program(const node &outer_functor);
    std::vector<const node *> flatten_query(const node &node);

    template<typename OutputIter>
    void
    to_query_instructions(const std::vector<const node *> &flattened_term, const node &outer_functor, OutputIter out,
                          std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                          storage& storage);

    template<typename OutputIter>
    void to_program_instructions(const std::vector<const node *> &flattened_term, OutputIter out,
            std::unordered_map<wam::helper::seen_register,bool> &seen_regs,
            wam::storage& storage);



    std::unordered_map<wam::functor_view, std::vector<wam::rule>>
    compile_program(std::string_view program_code, wam::storage& storage);
    /*
     * Parses a query term e.G. p(Z,h(Z,W),f(W))
     */
    wam::rule compile_query(std::string_view query_code, wam::storage& storage);

    void compile_query_atom(node &&atom,
                                 std::unordered_map<wam::helper::seen_register, bool> &seen_registers,
                                 std::vector<std::function<void(wam::executor &)>> &instructions,
                                 wam::rule &term_codes,
                                 bool from_original_query,
                                 storage& storage);
}


#endif //PROLOG_BFS_COMPILER_H
