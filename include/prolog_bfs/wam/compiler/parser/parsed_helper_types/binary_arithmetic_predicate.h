//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_BINARY_ARITHMETIC_PREDICATE_H
#define PROLOG_BFS_BINARY_ARITHMETIC_PREDICATE_H

#include <prolog_bfs/wam/compiler/util/data_enums.h>
#include <prolog_bfs/wam/compiler/util/node.h>
#include <boost/fusion/include/adapt_struct.hpp>

namespace parser {
    struct binary_arithmetic_predicate {
        std::string pred;
        node lhs;
        node rhs;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        parser::binary_arithmetic_predicate,
        (node, lhs)
        (std::string, pred)
        (node, rhs)
)

#endif //PROLOG_BFS_BINARY_ARITHMETIC_PREDICATE_H
