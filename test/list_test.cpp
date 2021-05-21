//
// Created by leonhard on 10.05.20.
//

#include "../catch.hpp"
#include <map>
#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "test_util.h"

using namespace std;
using namespace wam;

//These tests are not working yet

TEST_CASE("len of list tests 1") {
    bfs_organizer org;

    const char * prog =
            "len([X | R], Y) :- len(R, LR), Y is 1 + LR."
            "len([], 0)."
            "l([1]).";
    org.load_program(prog);
    org.load_query("l(X), len(X, Y)");
    auto ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
            "X", "[1]",
            "Y", "1"
    });
    org.get_unification_tree();
}
TEST_CASE("len of list tests") {
    bfs_organizer org;

    const char * prog =
            "len([X | R], Y) :- len(R, LR), Y is 1 + LR."
            "len([], 0)."
            "alt([1,0 | R]) :- alt(R)."
            "alt([]).";
    org.load_program(prog);
    org.load_query("alt(X), len(X, R)");
    auto ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
        "X", "[]",
        "R", "0"
    });
    ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
            "X", "[1,0]",
            "R", "2"
    });
    ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
            "X", "[1,0,1,0]",
            "R", "4"
    });
}

TEST_CASE("istdrin") {
    bfs_organizer org;

    const char * prog =
        "istdrin(X, [X|Xs])."
        "istdrin(X, [Y|Xs]) :- istdrin(X, Xs).";
    org.load_program(prog);
    org.load_query("istdrin(a, Zs)");
    auto ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
        "Zs", "[a,Xs]"
    });
    ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
        "Zs", "[Y3,a,Xs]"
    });
    ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
        "Zs", "[Y3,Y6,a,Xs1]"
    });
}
