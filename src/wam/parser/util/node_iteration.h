//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_NODE_ITERATION_H
#define PROLOG_BFS_NODE_ITERATION_H


#include <vector>
#include <queue>
#include <functional>

 #include "node.h"
//Util functions to make accessing nodes in a tree in a given order easier

template<typename Functor>
void bfs_order(const node &top_node, Functor func) {
    std::queue<const node*> queue;

    if(top_node.type == STORED_OBJECT_FLAG::NONE){
        queue.push(&top_node.children->operator[](0));
    } else{
        queue.push(&top_node);
    }


    while(!queue.empty()){
        const node* cur = queue.front();
        queue.pop();
        std::invoke(func, cur);

        if(cur->children){
            for(auto& node : *cur->children){
                queue.push(&node);
            }
        }
    }
}
template<typename Functor>
void bfs_order(node &top_node, Functor func) {
    std::queue<node*> queue;

    if(top_node.type == STORED_OBJECT_FLAG::NONE){
        queue.push(&top_node.children->operator[](0));
    } else{
        queue.push(&top_node);
    }


    while(!queue.empty()){
        node* cur = queue.front();
        queue.pop();
        std::invoke(func, cur);

        if(cur->children){
            for(auto& node : *cur->children){
                queue.push(&node);
            }
        }
    }
}

#endif //PROLOG_BFS_NODE_ITERATION_H
