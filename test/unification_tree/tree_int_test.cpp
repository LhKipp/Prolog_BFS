//
// Created by leonhard on 08.05.20.
//

#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("hard tree with integer") {
    bfs_organizer org;
    const char *text =
            "h(1)."
            "o(X) :- h(X)."
            "nat(2)."
            "nat(1)."
            "one(X) :- nat(X), o(X)."
            "two(2)."
            "l([a, b, [Y, [X]]], f(b([a, [a] | [Z]]))) :- one(Y), one(X), two(Z).";
    org.load_program(text);
    org.load_query("l([Y, b, [1, [X]]], f(b([a, [a] | [Z]]))).");
    auto ans = org.get_answer().get_answer();
    auto t = org.get_unification_tree();
}

