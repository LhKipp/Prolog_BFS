//
// Created by leonhard on 08.05.20.
//

#ifndef PROLOG_BFS_PROLOG_DATA_TYPES_H
#define PROLOG_BFS_PROLOG_DATA_TYPES_H

#include <vector>
#include <prolog_bfs/wam/compiler/util/data_enums.h>
#include <prolog_bfs/wam/compiler/util/node.h>
#include <boost/fusion/include/adapt_struct.hpp>

namespace parser{
    struct functor{
        std::string name;
        std::vector<node> children;
    };

    struct normal_list{
        std::vector<node> children;
    };

    struct finished_list{
        std::vector<node> children;
        node end;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
        parser::normal_list,
        (std::vector<node>, children))

BOOST_FUSION_ADAPT_STRUCT(
        parser::finished_list,
        (std::vector<node>, children),
        (node, end))

BOOST_FUSION_ADAPT_STRUCT(
        parser::functor,
                (std::string, name)
                        (std::vector<node>, children)

)
#endif //PROLOG_BFS_PROLOG_DATA_TYPES_H
