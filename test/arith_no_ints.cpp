//
// Created by leonhard on 19.08.19.
//
#include <catch2/catch.hpp>
#include <iostream>
#include <map>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("arithmetics tests") {
    bfs_organizer org;

    org.load_program_from_file("test_src/arithmetic.pl");

    SECTION("a") {
        org.load_query("mult(s(o),s(s(s(o))),Z).");
        auto ans = org.get_answer().get_answer();
        REQUIRE(ans.has_value());
        REQUIRE(ans->at(0).binding == "s(s(s(o)))");
    }
}



