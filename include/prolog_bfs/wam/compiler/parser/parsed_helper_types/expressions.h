//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_EXPRESSIONS_H
#define PROLOG_BFS_EXPRESSIONS_H

#include <vector>
#include <optional>
#include <prolog_bfs/wam/compiler/util/data_enums.h>
#include <prolog_bfs/wam/compiler/util/node.h>
#include <boost/fusion/include/adapt_struct.hpp>

namespace parser {

    struct chained_value{
        std::string math_op;
        node val;
    };

    struct chained_expr{
        node value;
        std::vector<chained_value> chained_values;
    };

    struct opt_chained_value{
        node value;
        std::optional<chained_value> chained_val;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        parser::chained_value,
        (std::string, math_op)
        (node, val)
)
BOOST_FUSION_ADAPT_STRUCT(
        parser::chained_expr,
        (node, value)
        (std::vector<parser::chained_value>, chained_values)
)
BOOST_FUSION_ADAPT_STRUCT(
        parser::opt_chained_value,
        (node, value)
        (std::optional<parser::chained_value>, chained_val)
)

#endif //PROLOG_BFS_EXPRESSIONS_H
