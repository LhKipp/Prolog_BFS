//
// Created by leonhard on 17.12.19.
//

#ifndef PROLOG_BFS_PROGRAM_LINE_H
#define PROLOG_BFS_PROGRAM_LINE_H


#include <boost/optional.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <ostream>
#include "../node.h"

/**
 * I think this would be a better datalayout than node tree.
 * But as long as parser.cpp is not refactored, we go with node tree
 *
 * THIS FILE IS NOT BEEING USED RIGHT NOW
 */
namespace wam {
    struct program_line {
        node head;
        boost::optional<std::vector<node>> body;

        /**
         * Returns this program_line in the old format
         * e.g. With top node of type NO_STORED_OBJECT, head as first child, and body nodes as childs [1..n]
         * @return the program_line as one node
         */
        node as_one_node(){
            node top_node{STORED_OBJECT_FLAG::NONE};
            size_t total_size = 1 + (body ? body->size() : 0);

            top_node.children = std::make_unique<std::vector<node>>();
            top_node.children->reserve(total_size);
            top_node.children->push_back(head);
            if(body){
                std::copy(body->cbegin(), body->cend(), std::back_insert_iterator(*top_node.children));
            }

            return top_node;
        }

        friend std::ostream &operator<<(std::ostream &os, const program_line &line) {
            os << "head: " << line.head;
            return os;
        }
    };


}

BOOST_FUSION_ADAPT_STRUCT(
        wam::program_line,
        (node, head)
                (boost::optional<std::vector<node>>, body)
)


#endif //PROLOG_BFS_PROGRAM_LINE_H
