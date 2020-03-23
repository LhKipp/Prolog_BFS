//
// Created by leonhard on 23.03.20.
//

#ifndef PROLOG_BFS_NODE_VAR_UTIL_H
#define PROLOG_BFS_NODE_VAR_UTIL_H

#include <cstddef>
#include <wam/compiler/util/node.h>

struct node_var_hasher{
    size_t operator()(const node& node)const{
        assert(node.is_variable());
        return std::hash<std::string>{}(node.name) * 21 ^ node.get_heap_index();
    }
};

struct node_var_equality{
    size_t operator()(const node& a, const node& b)const{
        assert(a.is_variable());
        assert(b.is_variable());
        return a.name == b.name && //Same names
               a.get_heap_index() == b.get_heap_index();//Same indexes in heap
    }
};
#endif //PROLOG_BFS_NODE_VAR_UTIL_H
