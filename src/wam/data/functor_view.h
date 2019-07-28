//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_FUNCTOR_VIEW_H
#define PROLOG_BFS_FUNCTOR_VIEW_H

#include <string>

struct functor_view{
    std::string name;
    int arity;

    functor_view()= default;
    functor_view(std::string name, int arity): name{name} , arity{arity}{}
};

#endif //PROLOG_BFS_FUNCTOR_VIEW_H
