//
// Created by leonhard on 08.03.20.
//
#include "../../catch.hpp"
#include "../../src/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>
#include <iostream>

using namespace wam;
using namespace std;

TEST_CASE("Tree mult") {
    bfs_organizer org;

    org.load_program_from_file("test_src/arithmetic.pl");

    org.load_query("mult(s(o),s(s(s(o))),Z).");

    auto ans = org.get_answer();
    query_node t = org.get_unification_tree();

    REQUIRE(t.get_query_as_str() == "mult(s(o),s(s(s(o))),Z)");
    REQUIRE(!t.failed());
    REQUIRE(t.is_from_orig_query());

    std::vector<var_binding_node>& facts = t.get_children();
    assert(facts.size() == 2);
    assert(facts[0].get_fact_as_str() == "mult(o, X, o)");
    assert(facts[0].failed());
    assert(!facts[0].succeeded());
    assert(!facts[0].continues());

    var_binding_node& continuation = facts[1];
    assert(continuation.continues());
    assert(continuation.get_fact_as_str() == "mult(s(A), B, C)");
    auto& bindings = continuation.get_var_bindings();

    vector<var_binding> solutions;
    solutions.emplace_back("Z","G_16" );
    solutions.emplace_back("B","s(s(s(o)))");
    solutions.emplace_back("C","G_16");
    solutions.emplace_back("A","o");
    for (auto &elem : bindings) {
//        std::cout << elem<< endl;
        auto found = std::find(solutions.begin(), solutions.end(), elem);
        bool has_found = found != solutions.end();
        REQUIRE(has_found);
        solutions.erase(found);
    }
    REQUIRE(!continuation.get_var_bindings_as_str().empty());
    REQUIRE(solutions.empty());

    const auto& q2 = continuation.get_continuing_query();
    REQUIRE(!q2.failed());
    REQUIRE(q2.get_query_as_str() == "mult(A, B, D)");
    REQUIRE(!q2.is_from_orig_query());
    REQUIRE(q2.get_children().size() == 2);

    auto& mult_fact = q2.get_children()[0];
    REQUIRE(mult_fact.get_fact_as_str() == "mult(o, X, o)");
    REQUIRE(mult_fact.continues());
    REQUIRE(mult_fact.continues());

    const auto& bindings2 = mult_fact.get_var_bindings();
    solutions.emplace_back("D","o" );
    solutions.emplace_back("A","o" );
    solutions.emplace_back("X","s(s(s(o)))");
    solutions.emplace_back("B","s(s(s(o)))");
    for (const auto &elem : bindings2) {
//        std::cout << elem<< endl;
        auto found = std::find(solutions.begin(), solutions.end(), elem);
        bool has_found = found != solutions.end();
        REQUIRE(has_found);
        solutions.erase(found);
    }




}


TEST_CASE("Tree fuzzing"){
    bfs_organizer org;
    org.load_program_from_file("test_src/p5-2.pl");
    org.load_query("lVonM(Z).");

    for (int i = 0; i < 5; ++i) {
        org.get_answer();
        org.get_unification_tree();
    }
}
