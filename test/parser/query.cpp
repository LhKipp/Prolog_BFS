//
// Created by leonhard on 07.03.20.
//

#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("Validate wrong query Code", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_query("DFS, LKJ, f(b), d.");
    REQUIRE(r.exists());
    REQUIRE(r.get_row() == 1);
    REQUIRE(r.get_col() == 1);
}

TEST_CASE("Validate correct query Code", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_query("f(a), f(b), d.");
    REQUIRE(!r.exists());
}

TEST_CASE("Parser query aa).", "[validate]") {
    bfs_organizer org;
    auto r = org.validate_query("aa).");
    REQUIRE(r.exists());
    REQUIRE(r.get_row() == 1);
    REQUIRE(r.get_col() == 3);
}
