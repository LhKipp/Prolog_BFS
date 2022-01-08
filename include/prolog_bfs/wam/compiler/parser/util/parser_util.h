//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_PARSER_UTIL_H
#define PROLOG_BFS_PARSER_UTIL_H

#include <string>
#include <vector>
#include "prolog_bfs/wam/compiler/util/node.h"

namespace wam {
    void make_to_children(node& top_node, std::vector<node>& children);
    void make_to_query(node& top_node, std::vector<node>& atoms);

    template<typename Iter>
    void add_source_code_info(node& node, Iter begin, Iter end);

}

template<typename Iter>
void wam::add_source_code_info(node &node, Iter begin, Iter end) {
    //For lower_bound info see boost spirit funcs
    auto& code_info = node.code_info;
    code_info.line_begin = get_line(begin);
    code_info.line_end = get_line(end);
    code_info.value = std::string{begin, end};
    //To implement get_column a lower_bound would be needed, that is
    //At least at the line_begin start or before. if lower_bound (as e.G. begin)
    //would be on the same line_begin, it will give wrong results
    //So for now we let it be
    //atom_with_err.column = get_column(lower_bound, begin) -1;
}


#endif //PROLOG_BFS_BFS_UTIL_H
