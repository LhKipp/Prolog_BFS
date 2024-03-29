#include <catch2/catch.hpp>
#include "test_util.h"
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <string>



using namespace wam;
using namespace std;

void make_is_test_case(std::string rhs, int rhs_val){
    bfs_organizer org;
    org.load_query("X is " + rhs);
    auto ans1 = org.get_answer();
    auto ans = ans1.get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == std::to_string(rhs_val));

    org.load_query(to_string(rhs_val) + " is " + rhs);
    auto ans2 = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

TEST_CASE("Is correct assignment") {
    make_is_test_case("3.", 3);
    make_is_test_case("3 + 3.", 6);
    make_is_test_case("3 - 4.", -1);
    make_is_test_case("3 * 3.", 9);
    make_is_test_case("9 // 3.", 3);
    make_is_test_case("2^3.", 8);
    make_is_test_case("3 * 3 - 1.", 8);
    make_is_test_case("(4 + 4) * 4 - ((2 + 3)*2).", 22);
    make_is_test_case("2 ^ 3 * 4.", 32);
    make_is_test_case("(((3^2)-1)*1+2) - 10.", 0);
}

TEST_CASE("Variable is assigment"){
    bfs_organizer org;
    org.load_query("X is (4 + 4) * 4 - ((2 + 3)*2).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == std::to_string(22));
}

TEST_CASE("Variable is assigment 2"){
    bfs_organizer org;
    const char* prog =
            "e(1 + 1).";
    org.load_program(prog);
    org.load_query("e(Y), X is Y");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(1).binding == "(1 + 1)");
    REQUIRE(ans->at(0).binding == std::to_string(2));
}

TEST_CASE("is comparison rules"){
    bfs_organizer org;
    //THIS IS THE BEHAVIOR OF SWIPL AND NOT NECESSARILY THE OPINION OF THE AUTHOR
    auto ass_false = [&](auto q){
        org.load_query(q);
        auto ans = org.get_answer().get_answer();
        REQUIRE(!ans.has_value());
    };
    ass_false("[] is 3.");
    ass_false("a is 3.");
    ass_false("f(a) is 3.");
}

TEST_CASE("Expression builder"){
    bfs_organizer org;
    const char* prog =
            "add(X, X + 1)."
            "min(X, X - 1)."
            "double(X, X * 2)."
            "div(X, X // 2).";
    org.load_program(prog);
    org.load_query("add(X, A),"
                   "add(A, B),"
                   "double(B, C),"
                   "double(C, D),"
                   "min(D, E),"
                   "div(E, R), X is 1, V is R.");
    auto ans = org.get_answer().get_answer();
    has_all_of_these(ans,
                     {"X" , "1",
                      "A", "(1 + 1)",
                      "B", "((1 + 1) + 1)",
                      "C", "(((1 + 1) + 1) * 2)",
                      "D", "((((1 + 1) + 1) * 2) * 2)",
                      "E", "(((((1 + 1) + 1) * 2) * 2) - 1)",
                      "R", "((((((1 + 1) + 1) * 2) * 2) - 1) // 2)",
                      "V", "5"});

    auto tree = org.get_unification_tree();
    REQUIRE(tree.get_resolved_query_as_str() == "add(X, A)");
    auto cont = tree.get_children()[0];
    auto q2 = cont.get_continuing_query();
    REQUIRE(q2.get_resolved_query_as_str() == "add((X + 1), B)");
    auto cont2 = q2.get_children()[0];
    auto q3 = cont2.get_continuing_query();
    REQUIRE(q3.get_resolved_query_as_str() == "double(((X + 1) + 1), C)");
}

TEST_CASE("Expression builder 2") {
    bfs_organizer org;
    const char *prog =
            "add(X, X + 1)."
            "min(X, X - 1)."
            "double(X, X * 2)."
            "div(X, X // 2).";
    org.load_program(prog);
    org.load_query("X is 3,"
                   "double(X, R),"
                   "add(R, R2),"
                   "div(R2, R3),"
                   "Y is R3");
    auto ans = org.get_answer().get_answer();
    has_all_of_these(ans,
                     {"X", "3",
                      "R", "(3 * 2)",
                      "R2", "((3 * 2) + 1)",
                      "R3", "(((3 * 2) + 1) // 2)",
                      "Y", "3"});
}
