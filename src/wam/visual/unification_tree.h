//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_UNIFICATION_TREE_H
#define PROLOG_BFS_UNIFICATION_TREE_H

#include "../data/term_code.h"

struct unification_tree{
    int node_id;
    wam::term_code* query;

};
#endif //PROLOG_BFS_UNIFICATION_TREE_H
