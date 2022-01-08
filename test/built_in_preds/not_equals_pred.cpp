#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <string>

using namespace wam;
using namespace std;

TEST_CASE("not_equals Functor inequality") {
    bfs_organizer org;

    org.load_program("eq(X, Y) :- X \\== Y.");
    org.load_query("eq(b, a).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

TEST_CASE("not_equals Functor inequality 2") {
    bfs_organizer org;
    const char *text =
            "a(a)."
            "b(b)."
            "eq(X,Y) :- a(X), b(Y), X \\== Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

TEST_CASE("not_equals Functor equality") {
    bfs_organizer org;
    const char *text =
            "a(a)."
            "b(a)."
            "eq(X,Y) :- a(X), b(Y), X \\== Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
}

TEST_CASE("not_equals Functor equality 2") {
    bfs_organizer org;
    const char *text =
            "a(x(d,f))."
            "b(x(d,f))."
            "eq(X,Y) :- a(X), b(Y), X \\== Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
}

TEST_CASE("not_equals Functor equality with var") {
    bfs_organizer org;
    const char *text =
            "a(x(H,f))."
            "b(x(H,f))."
            "eq(X,Y) :- a(X), b(Y), X \\== Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

TEST_CASE("not_equals Var inequality") {
    bfs_organizer org;
    org.load_query("X \\== X.");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
}

TEST_CASE("not_equals Var inequality2") {
    bfs_organizer org;
    org.load_query("Y \\== X.");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

//TODO this code doenst compile under swipl because X is a singleton variable
//But it does make sense so???
TEST_CASE("not_equals Var inequality 2") {
    bfs_organizer org;
    const char *text =
            "a(X)."
            "b(X)."
            "eq(X,Y) :- a(X), b(Y), X \\== Y.";
    org.load_program(text);
    org.load_query("eq(A,B).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}
