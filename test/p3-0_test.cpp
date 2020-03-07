//
// Created by leonhard on 18.08.19.
//
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p3-0 test") {
    bfs_organizer org;

    org.load_program_from_file("test_src/p3-0.pl");

    SECTION("lVonM") {

        org.load_query("roots(Z1, Z2).");

        for (int i = 0; i < 25; i++) {
            auto answer = org.get_answer();
            REQUIRE(answer.has_value());
            for (auto &elem : *answer) {
//                cout << elem.binding << endl;
            }
        }


    }

}



