//

#include "prolog_bfs/wam/compiler/parser/util/parser_util.h"

void wam::make_to_query(node& top_node, std::vector<node>& atoms){
    top_node.children = std::make_unique<std::vector<node>>(atoms);
    top_node.code_info.line_begin = std::numeric_limits<unsigned>::max();
    top_node.code_info.line_end = std::numeric_limits<unsigned>::max();
    //For now we emit whole query as string, because not needed
}

void wam::make_to_children(node& top_node, std::vector<node>& children){
    top_node.children = std::make_unique<std::vector<node>>(children);
}
