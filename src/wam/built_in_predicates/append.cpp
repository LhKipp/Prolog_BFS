//
// Created by leonhard on 16.05.20.
//

#include "prolog_bfs/wam/built_in_predicates/append.h"

std::vector<node> wam::preds::get_append_node_tree() {
    node fact{STORED_OBJECT_FLAG ::NONE};
    fact.children = std::make_unique<std::vector<node>>();

    //fact append([], List2, List2).
    node append{STORED_OBJECT_FLAG ::FUNCTOR, "append"};
    append.code_info = code_info_for_built_in("append([], List2, List2)");
    append.children->push_back(node{STORED_OBJECT_FLAG ::FUNCTOR, "["});
    append.children->push_back(node{STORED_OBJECT_FLAG ::VARIABLE, "List2"});
    append.children->push_back(node{STORED_OBJECT_FLAG ::VARIABLE, "List2"});
    fact.children->emplace_back(append);

    node rule{STORED_OBJECT_FLAG ::NONE};
    rule.children = std::make_unique<std::vector<node>>();

    //rule append([X | Xs], Ys, [X | Rs]) :- append(Xs, Ys, Rs).
    node clause_head{STORED_OBJECT_FLAG ::FUNCTOR, "append"};
    clause_head.code_info = code_info_for_built_in("append([X | Xs], Ys, [X | Rs])");
    node x_list{STORED_OBJECT_FLAG ::FUNCTOR, "["};
    x_list.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "X");
    x_list.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Xs");
    node result_list{STORED_OBJECT_FLAG ::FUNCTOR, "["};
    result_list.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "X");
    result_list.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Rs");

    clause_head.children->emplace_back(x_list);
    clause_head.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Ys");
    clause_head.children->emplace_back(result_list);

    node body_atom{STORED_OBJECT_FLAG ::FUNCTOR, "append"};
    body_atom.code_info = code_info_for_built_in("append(Xs, Ys, Rs)");
    body_atom.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Xs");
    body_atom.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Ys");
    body_atom.children->emplace_back(STORED_OBJECT_FLAG::VARIABLE, "Rs");

    rule.children->emplace_back(clause_head);
    rule.children->emplace_back(body_atom);

    return {fact, rule};
}
