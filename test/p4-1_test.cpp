// Created by leonhard on a8.b8.a9.
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p4-1 test") {
    bfs_organizer org;

    org.load_program("test_src/p4-1.pl");

    SECTION("[]") {
        org.load_query("lvongbb([b,a]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("[a]") {
        org.load_query("lvongbb([b,b,b,a]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("lvongbb") {

        vector<string> solutions;

        org.load_query("lvongbb(Z).");

//        for (int i = 0; i < 10; i++) {
//            auto answer = org.get_answer();
//            REQUIRE(answer.has_value());
//            for (auto &elem : *answer) {
//                /* std::cout << elem<< endl; */
////                auto found = std::find(solutions.begin(), solutions.end(), elem.substitute);
////                bool has_found = found != solutions.end();
////                REQUIRE(has_found);
//            }
//        }
        /* REQUIRE(solutions.empty()); */


    }

}



