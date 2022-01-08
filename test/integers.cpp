//
// Created by leonhard on 07.05.20.
//
#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include "./test_util.h"

using namespace std;
using namespace wam;

TEST_CASE("easy unification with integer") {
    bfs_organizer org;
    const char *text =
            "a(1).";
    org.load_program(text);
    org.load_query("a(X).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == "1");
}

TEST_CASE("hard unification with integer") {
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
    has_all_of_these(ans,
                     {"Y", "a",
                      "X", "1",
                      "Z", "2"});
}

TEST_CASE("altering with integer") {
    bfs_organizer org;
    const char *text =
            "alt([])."
            "alt([1, 0 | R]) :- alt(R).";
    org.load_program(text);
    org.load_query("alt(X).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans->at(0).binding == "[]");
    auto exc2 = org.get_answer();
    auto ans2 = exc2.get_answer();
    REQUIRE(ans2->at(0).binding == "[1,0]");
    auto ans3 = org.get_answer().get_answer();
    REQUIRE(ans3->at(0).binding == "[1,0,1,0]");
}
