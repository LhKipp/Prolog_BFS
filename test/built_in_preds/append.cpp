#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <string>

using namespace wam;
using namespace std;

TEST_CASE("Ez append") {
    bfs_organizer org;
    org.load_query("append([], [a], [a])");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    org.get_unification_tree();
}

TEST_CASE("Hard append") {
    bfs_organizer org;
    org.load_query("append([a,[X],c], [1], Z).");
    auto ans = org.get_answer().get_answer();
    org.get_unification_tree();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == "[a,[X],c,1]");
}

TEST_CASE("Redefinition of append"){
    bfs_organizer org;
    const char * prog =
            "append(bla, bla, bla).";
    auto err = org.load_program(prog);
    REQUIRE(err.exists());
    REQUIRE(err.type == compiler::ERROR_TYPE::REDEFINITION_OF_BUILT_IN_PREDICATE);
}
