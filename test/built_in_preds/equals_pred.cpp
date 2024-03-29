#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <string>

using namespace wam;
using namespace std;

void make_eq_test_case(std::string q, bool ans_expected){
    bfs_organizer org;
    org.load_query(q);
    auto ans1 = org.get_answer().get_answer();
    REQUIRE(ans1.has_value() == ans_expected);
    auto t1 = org.get_unification_tree();
}
TEST_CASE("Functor Equality") {
    bfs_organizer org;

    org.load_program("eq(X, Y) :- X == Y.");
    org.load_query("eq(a, a).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Functor Equality 2") {
    bfs_organizer org;
    const char *text =
            "a(a)."
            "b(a)."
            "eq(X,Y) :- a(X), b(Y), X == Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    org.get_unification_tree();
}


TEST_CASE("Functor inequality") {
    bfs_organizer org;
    const char *text =
            "a(a)."
            "b(b)."
            "eq(X,Y) :- a(X), b(Y), X == Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Functor inequality 2") {
    bfs_organizer org;
    const char *text =
            "a(x(d,f))."
            "b(x(e,f))."
            "eq(X,Y) :- a(X), b(Y), X == Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Functor inequality with var") {
    bfs_organizer org;
    const char *text =
            "a(x(H,f))."
            "b(x(G,f))."
            "eq(X,Y) :- a(X), b(Y), X == Y.";
    org.load_program(text);
    org.load_query("eq(X, Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Var equality") {
    bfs_organizer org;
    org.load_query("X == X.");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Var inequality") {
    bfs_organizer org;
    org.load_query("Y == X.");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
    org.get_unification_tree();
}

//TODO this code doenst compile under swipl because X is a singleton variable
//But it does make sense so???
TEST_CASE("Var inequality 2") {
    bfs_organizer org;
    const char *text =
            "a(X)."
            "b(X)."
            "eq(X,Y) :- a(X), b(Y), X == Y.";
    org.load_program(text);
    org.load_query("eq(A,B).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(!ans.has_value());
    org.get_unification_tree();
}


TEST_CASE("Var equality tree") {
    bfs_organizer org;
    org.load_program("eq(X, X) :- X == X.");
    org.load_query("eq(X, X).");
    auto ans1 = org.get_answer().get_answer();
    REQUIRE(ans1.has_value());
    auto t1 = org.get_unification_tree();

    org.load_query("X == X.");
    auto ans = org.get_answer().get_answer();
    auto t = org.get_unification_tree();
    REQUIRE(ans.has_value());
}

TEST_CASE("Int equality"){
    make_eq_test_case("3 == 3.", true);
}

TEST_CASE("Int equality false"){
    make_eq_test_case("3 ==4.", false);
}

TEST_CASE("Cons equality"){
    make_eq_test_case("a == a.", true);
}

TEST_CASE("Cons inequality"){
    make_eq_test_case("a == b.", false);
}
