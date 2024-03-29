//
// Created by leonhard on 18.08.19.
//
#include <iostream>
#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("weg laenge test") {
    bfs_organizer org;

    org.load_program_from_file("test_src/wegLange1.pl");

    SECTION("[]") {
        org.load_query("wegLaenge(a,f,Z).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->at(0).binding == "s(s(s(o)))");
    }
    SECTION("[]") {
        org.load_query("wegLaenge(a,d,Z).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->at(0).binding == "s(s(o))");
    }


}



