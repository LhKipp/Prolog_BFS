//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_NODE_ITERATION_H
#define PROLOG_BFS_NODE_ITERATION_H


#include <vector>
#include <queue>
#include <functional>

template<typename node, typename Functor>
void bfs_order(node &top_node, bool skip_top_node, Functor func) {
    std::queue<node *> queue;

    if (skip_top_node) {
        for (auto &child : *top_node.children)
            queue.push(&child);
    } else {
        queue.push(&top_node);
    }


    while (!queue.empty()) {
        node *cur = queue.front();
        queue.pop();
        std::invoke(func, cur);

        if (cur->children) {
            for (auto &cur_node : *cur->children) {
                queue.push(&cur_node);
            }
        }
    }
}

#endif //PROLOG_BFS_NODE_ITERATION_H
