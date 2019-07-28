//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_PARSER_H
#define PROLOG_BFS_PARSER_H


#include <string>
#include "util/node.h"
#include "../executor/executor.h"

namespace wam {


    /*
     * Parses a query term e.G. p(Z,h(Z,W),f(W))
     */
    void parse_query(wam::executor &executor, const std::string &line);


    void assign_registers(executor &executor, node &node);

    node build_tree(const std::string &line);


    std::vector<const node *> flatten_query(executor &executor, const node &node);
}


#endif //PROLOG_BFS_PARSER_H
