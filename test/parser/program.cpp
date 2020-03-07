//
// Created by leonhard on 07.03.20.
//

#include "../../catch.hpp"
#include "../../src/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("Validate correct program Code", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_program("f(a) :- f(b).");
    REQUIRE(!r.exists());
}
TEST_CASE("Validate wrong program Code", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_program("f(a) :- f(b) X.\nf(a) :- f(b), f(c)");
    REQUIRE(r.get_row() == 0);
    REQUIRE(r.get_col() == 13);
    REQUIRE(r.exists());
}

TEST_CASE("Parser program aa).", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_program("aa).");
    REQUIRE(r.exists());
    REQUIRE(r.get_row() == 0);
    REQUIRE(r.get_col() == 2);
}
