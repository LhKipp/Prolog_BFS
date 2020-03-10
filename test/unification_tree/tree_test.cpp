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

    REQUIRE(t.get_query_as_string() == "mult(s(o),s(s(s(o))),Z)");
    REQUIRE(!t.failed());
    REQUIRE(t.is_from_orig_query());

    std::vector<var_binding_node>& facts = t.getChildren();
    assert(facts.size() == 2);
    assert(facts[0].get_fact_as_string() == "mult(o, X, o)");
    assert(facts[0].failed());
    assert(!facts[0].succeeded());
    assert(!facts[0].continues());

    var_binding_node& continuation = facts[1];
    assert(continuation.continues());
    assert(continuation.get_fact_as_string() == "mult(s(A), B, C)");


}


