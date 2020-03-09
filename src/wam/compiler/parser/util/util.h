//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_UTIL_H
#define PROLOG_BFS_UTIL_H

#include <string>
#include <boost/fusion/include/at_c.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include "../../util/node.h"

namespace wam {

    template<typename fusion_vec>
    std::string boost_vec_to_string(fusion_vec vec);


    void make_to_top_node(boost::spirit::unused_type unused, node &n);

    void make_to_cons(std::string &name, node &node);

    void make_to_func(std::string_view name, node &n);

    void make_to_var(std::string_view name, node &n);

    void make_to_list(char name, node &n);

    void set_list_finished(char unused_attribute, node &node);

    void childs_to_list(node &list_start, char unused_attribute);
}

#endif //PROLOG_BFS_UTIL_H
