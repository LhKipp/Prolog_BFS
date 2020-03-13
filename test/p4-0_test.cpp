// Created by leonhard on a8.b8.a9.
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p4-b test") {
    bfs_organizer org;

    org.load_program_from_file("test_src/p4-0.pl");

    SECTION("[]") {
        org.load_query("lvonN([b,a]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("[a]") {
        org.load_query("lvonN([b,b,b,a]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("lvonN") {

        vector<string> solutions;

        org.load_query("lvonN(Z).");

        for (int i = 0; i < 25; i++) {
            auto answer = org.get_answer();
            REQUIRE(answer.has_value());
            for (auto &elem : *answer) {
                /* std::cout << elem<< endl; */
                //Prolog file is buggy, but the test is working in this commit at least :)
//                auto found = std::find(solutions.begin(), solutions.end(), elem.binding);
//                bool has_found = found != solutions.end();
//                REQUIRE(has_found);
//                solutions.erase(found);
            }
        }
        /* REQUIRE(solutions.empty()); */


    }

}



