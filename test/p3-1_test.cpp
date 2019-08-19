//
// Created by leonhard on 18.08.19.
//
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p3-1 test") {
    bfs_organizer org;

    org.load_program("test_src/p5-0.pl");

    SECTION("[]") {
        org.load_query("lvonM([]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("[a]") {
        org.load_query("lvonM([a]).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("lVonM") {

        org.load_query("lvonM(Z).");
        vector<string> solutions;
        solutions.push_back("[]");
        solutions.push_back("[nix]");
        solutions.push_back("[b]");
        solutions.push_back("[a]");
        solutions.push_back("[nix,b]");
        solutions.push_back("[nix,a]");
        solutions.push_back("[b,b]");
        solutions.push_back("[b,nix,b]");
        solutions.push_back("[a,a]");
        solutions.push_back("[a,nix,a]");
        solutions.push_back("[nix,b,b]");
        solutions.push_back("[nix,b,nix,b]");
        solutions.push_back("[nix,a,a]");
        solutions.push_back("[nix,a,nix,a]");
        solutions.push_back("[b,b,b]");
        solutions.push_back("[b,a,b]");
        solutions.push_back("[b,nix,b,b]");
        solutions.push_back("[b,nix,a,b]");
        solutions.push_back("[a,b,a]");
        solutions.push_back("[a,a,a]");
        solutions.push_back("[a,nix,b,a]");
        solutions.push_back("[a,nix,a,a]");
        solutions.push_back("[nix,b,b,b]");
        solutions.push_back("[nix,b,a,b]");
        solutions.push_back("[nix,b,nix,b,b]");
        solutions.push_back("[nix,b,nix,a,b]");
        solutions.push_back("[nix,a,b,a]");
        solutions.push_back("[nix,a,a,a]");
        solutions.push_back("[nix,a,nix,b,a]");
        solutions.push_back("[nix,a,nix,a,a]");
        solutions.push_back("[b,b,b,b]");
        solutions.push_back("[b,b,nix,b,b]");
        solutions.push_back("[b,a,a,b]");
        solutions.push_back("[b,a,nix,a,b]");
        solutions.push_back("[b,nix,b,b,b]");
        solutions.push_back("[b,nix,b,nix,b,b]");
        solutions.push_back("[b,nix,a,a,b]");
        solutions.push_back("[b,nix,a,nix,a,b]");
        solutions.push_back("[a,b,b,a]");
        solutions.push_back("[a,b,nix,b,a]");
        solutions.push_back("[a,a,a,a]");
        solutions.push_back("[a,a,nix,a,a]");
        solutions.push_back("[a,nix,b,b,a]");
        solutions.push_back("[a,nix,b,nix,b,a]");
        solutions.push_back("[a,nix,a,a,a]");
        solutions.push_back("[a,nix,a,nix,a,a]");
        solutions.push_back("[nix,b,b,b,b]");
        solutions.push_back("[nix,b,b,nix,b,b]");
        solutions.push_back("[nix,b,a,a,b]");
        solutions.push_back("[nix,b,a,nix,a,b]");
        solutions.push_back("[nix,b,nix,b,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,b,b]");
        solutions.push_back("[nix,b,nix,a,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,a,b]");
        solutions.push_back("[nix,a,b,b,a]");
        solutions.push_back("[nix,a,b,nix,b,a]");
        solutions.push_back("[nix,a,a,a,a]");
        solutions.push_back("[nix,a,a,nix,a,a]");
        solutions.push_back("[nix,a,nix,b,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,b,a]");
        solutions.push_back("[nix,a,nix,a,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,a,a]");
        solutions.push_back("[b,b,b,b,b]");
        solutions.push_back("[b,b,a,b,b]");
        solutions.push_back("[b,b,nix,b,b,b]");
        solutions.push_back("[b,b,nix,a,b,b]");
        solutions.push_back("[b,a,b,a,b]");
        solutions.push_back("[b,a,a,a,b]");
        solutions.push_back("[b,a,nix,b,a,b]");
        solutions.push_back("[b,a,nix,a,a,b]");
        solutions.push_back("[b,nix,b,b,b,b]");
        solutions.push_back("[b,nix,b,a,b,b]");
        solutions.push_back("[b,nix,b,nix,b,b,b]");
        solutions.push_back("[b,nix,b,nix,a,b,b]");
        solutions.push_back("[b,nix,a,b,a,b]");
        solutions.push_back("[b,nix,a,a,a,b]");
        solutions.push_back("[b,nix,a,nix,b,a,b]");
        solutions.push_back("[b,nix,a,nix,a,a,b]");
        solutions.push_back("[a,b,b,b,a]");
        solutions.push_back("[a,b,a,b,a]");
        solutions.push_back("[a,b,nix,b,b,a]");
        solutions.push_back("[a,b,nix,a,b,a]");
        solutions.push_back("[a,a,b,a,a]");
        solutions.push_back("[a,a,a,a,a]");
        solutions.push_back("[a,a,nix,b,a,a]");
        solutions.push_back("[a,a,nix,a,a,a]");
        solutions.push_back("[a,nix,b,b,b,a]");
        solutions.push_back("[a,nix,b,a,b,a]");
        solutions.push_back("[a,nix,b,nix,b,b,a]");
        solutions.push_back("[a,nix,b,nix,a,b,a]");
        solutions.push_back("[a,nix,a,b,a,a]");
        solutions.push_back("[a,nix,a,a,a,a]");
        solutions.push_back("[a,nix,a,nix,b,a,a]");
        solutions.push_back("[a,nix,a,nix,a,a,a]");
        solutions.push_back("[nix,b,b,b,b,b]");
        solutions.push_back("[nix,b,b,a,b,b]");
        solutions.push_back("[nix,b,b,nix,b,b,b]");
        solutions.push_back("[nix,b,b,nix,a,b,b]");
        solutions.push_back("[nix,b,a,b,a,b]");
        solutions.push_back("[nix,b,a,a,a,b]");
        solutions.push_back("[nix,b,a,nix,b,a,b]");
        solutions.push_back("[nix,b,a,nix,a,a,b]");
        solutions.push_back("[nix,b,nix,b,b,b,b]");
        solutions.push_back("[nix,b,nix,b,a,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,b,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,a,b,b]");
        solutions.push_back("[nix,b,nix,a,b,a,b]");
        solutions.push_back("[nix,b,nix,a,a,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,b,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,a,a,b]");
        solutions.push_back("[nix,a,b,b,b,a]");
        solutions.push_back("[nix,a,b,a,b,a]");
        solutions.push_back("[nix,a,b,nix,b,b,a]");
        solutions.push_back("[nix,a,b,nix,a,b,a]");
        solutions.push_back("[nix,a,a,b,a,a]");
        solutions.push_back("[nix,a,a,a,a,a]");
        solutions.push_back("[nix,a,a,nix,b,a,a]");
        solutions.push_back("[nix,a,a,nix,a,a,a]");
        solutions.push_back("[nix,a,nix,b,b,b,a]");
        solutions.push_back("[nix,a,nix,b,a,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,b,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,a,b,a]");
        solutions.push_back("[nix,a,nix,a,b,a,a]");
        solutions.push_back("[nix,a,nix,a,a,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,b,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,a,a,a]");
        solutions.push_back("[b,b,b,b,b,b]");
        solutions.push_back("[b,b,b,nix,b,b,b]");
        solutions.push_back("[b,b,a,a,b,b]");
        solutions.push_back("[b,b,a,nix,a,b,b]");
        solutions.push_back("[b,b,nix,b,b,b,b]");
        solutions.push_back("[b,b,nix,b,nix,b,b,b]");
        solutions.push_back("[b,b,nix,a,a,b,b]");
        solutions.push_back("[b,b,nix,a,nix,a,b,b]");
        solutions.push_back("[b,a,b,b,a,b]");
        solutions.push_back("[b,a,b,nix,b,a,b]");
        solutions.push_back("[b,a,a,a,a,b]");
        solutions.push_back("[b,a,a,nix,a,a,b]");
        solutions.push_back("[b,a,nix,b,b,a,b]");
        solutions.push_back("[b,a,nix,b,nix,b,a,b]");
        solutions.push_back("[b,a,nix,a,a,a,b]");
        solutions.push_back("[b,a,nix,a,nix,a,a,b]");
        solutions.push_back("[b,nix,b,b,b,b,b]");
        solutions.push_back("[b,nix,b,b,nix,b,b,b]");
        solutions.push_back("[b,nix,b,a,a,b,b]");
        solutions.push_back("[b,nix,b,a,nix,a,b,b]");
        solutions.push_back("[b,nix,b,nix,b,b,b,b]");
        solutions.push_back("[b,nix,b,nix,b,nix,b,b,b]");
        solutions.push_back("[b,nix,b,nix,a,a,b,b]");
        solutions.push_back("[b,nix,b,nix,a,nix,a,b,b]");
        solutions.push_back("[b,nix,a,b,b,a,b]");
        solutions.push_back("[b,nix,a,b,nix,b,a,b]");
        solutions.push_back("[b,nix,a,a,a,a,b]");
        solutions.push_back("[b,nix,a,a,nix,a,a,b]");
        solutions.push_back("[b,nix,a,nix,b,b,a,b]");
        solutions.push_back("[b,nix,a,nix,b,nix,b,a,b]");
        solutions.push_back("[b,nix,a,nix,a,a,a,b]");
        solutions.push_back("[b,nix,a,nix,a,nix,a,a,b]");
        solutions.push_back("[a,b,b,b,b,a]");
        solutions.push_back("[a,b,b,nix,b,b,a]");
        solutions.push_back("[a,b,a,a,b,a]");
        solutions.push_back("[a,b,a,nix,a,b,a]");
        solutions.push_back("[a,b,nix,b,b,b,a]");
        solutions.push_back("[a,b,nix,b,nix,b,b,a]");
        solutions.push_back("[a,b,nix,a,a,b,a]");
        solutions.push_back("[a,b,nix,a,nix,a,b,a]");
        solutions.push_back("[a,a,b,b,a,a]");
        solutions.push_back("[a,a,b,nix,b,a,a]");
        solutions.push_back("[a,a,a,a,a,a]");
        solutions.push_back("[a,a,a,nix,a,a,a]");
        solutions.push_back("[a,a,nix,b,b,a,a]");
        solutions.push_back("[a,a,nix,b,nix,b,a,a]");
        solutions.push_back("[a,a,nix,a,a,a,a]");
        solutions.push_back("[a,a,nix,a,nix,a,a,a]");
        solutions.push_back("[a,nix,b,b,b,b,a]");
        solutions.push_back("[a,nix,b,b,nix,b,b,a]");
        solutions.push_back("[a,nix,b,a,a,b,a]");
        solutions.push_back("[a,nix,b,a,nix,a,b,a]");
        solutions.push_back("[a,nix,b,nix,b,b,b,a]");
        solutions.push_back("[a,nix,b,nix,b,nix,b,b,a]");
        solutions.push_back("[a,nix,b,nix,a,a,b,a]");
        solutions.push_back("[a,nix,b,nix,a,nix,a,b,a]");
        solutions.push_back("[a,nix,a,b,b,a,a]");
        solutions.push_back("[a,nix,a,b,nix,b,a,a]");
        solutions.push_back("[a,nix,a,a,a,a,a]");
        solutions.push_back("[a,nix,a,a,nix,a,a,a]");
        solutions.push_back("[a,nix,a,nix,b,b,a,a]");
        solutions.push_back("[a,nix,a,nix,b,nix,b,a,a]");
        solutions.push_back("[a,nix,a,nix,a,a,a,a]");
        solutions.push_back("[a,nix,a,nix,a,nix,a,a,a]");
        solutions.push_back("[nix,b,b,b,b,b,b]");
        solutions.push_back("[nix,b,b,b,nix,b,b,b]");
        solutions.push_back("[nix,b,b,a,a,b,b]");
        solutions.push_back("[nix,b,b,a,nix,a,b,b]");
        solutions.push_back("[nix,b,b,nix,b,b,b,b]");
        solutions.push_back("[nix,b,b,nix,b,nix,b,b,b]");
        solutions.push_back("[nix,b,b,nix,a,a,b,b]");
        solutions.push_back("[nix,b,b,nix,a,nix,a,b,b]");
        solutions.push_back("[nix,b,a,b,b,a,b]");
        solutions.push_back("[nix,b,a,b,nix,b,a,b]");
        solutions.push_back("[nix,b,a,a,a,a,b]");
        solutions.push_back("[nix,b,a,a,nix,a,a,b]");
        solutions.push_back("[nix,b,a,nix,b,b,a,b]");
        solutions.push_back("[nix,b,a,nix,b,nix,b,a,b]");
        solutions.push_back("[nix,b,a,nix,a,a,a,b]");
        solutions.push_back("[nix,b,a,nix,a,nix,a,a,b]");
        solutions.push_back("[nix,b,nix,b,b,b,b,b]");
        solutions.push_back("[nix,b,nix,b,b,nix,b,b,b]");
        solutions.push_back("[nix,b,nix,b,a,a,b,b]");
        solutions.push_back("[nix,b,nix,b,a,nix,a,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,b,b,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,b,nix,b,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,a,a,b,b]");
        solutions.push_back("[nix,b,nix,b,nix,a,nix,a,b,b]");
        solutions.push_back("[nix,b,nix,a,b,b,a,b]");
        solutions.push_back("[nix,b,nix,a,b,nix,b,a,b]");
        solutions.push_back("[nix,b,nix,a,a,a,a,b]");
        solutions.push_back("[nix,b,nix,a,a,nix,a,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,b,b,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,b,nix,b,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,a,a,a,b]");
        solutions.push_back("[nix,b,nix,a,nix,a,nix,a,a,b]");
        solutions.push_back("[nix,a,b,b,b,b,a]");
        solutions.push_back("[nix,a,b,b,nix,b,b,a]");
        solutions.push_back("[nix,a,b,a,a,b,a]");
        solutions.push_back("[nix,a,b,a,nix,a,b,a]");
        solutions.push_back("[nix,a,b,nix,b,b,b,a]");
        solutions.push_back("[nix,a,b,nix,b,nix,b,b,a]");
        solutions.push_back("[nix,a,b,nix,a,a,b,a]");
        solutions.push_back("[nix,a,b,nix,a,nix,a,b,a]");
        solutions.push_back("[nix,a,a,b,b,a,a]");
        solutions.push_back("[nix,a,a,b,nix,b,a,a]");
        solutions.push_back("[nix,a,a,a,a,a,a]");
        solutions.push_back("[nix,a,a,a,nix,a,a,a]");
        solutions.push_back("[nix,a,a,nix,b,b,a,a]");
        solutions.push_back("[nix,a,a,nix,b,nix,b,a,a]");
        solutions.push_back("[nix,a,a,nix,a,a,a,a]");
        solutions.push_back("[nix,a,a,nix,a,nix,a,a,a]");
        solutions.push_back("[nix,a,nix,b,b,b,b,a]");
        solutions.push_back("[nix,a,nix,b,b,nix,b,b,a]");
        solutions.push_back("[nix,a,nix,b,a,a,b,a]");
        solutions.push_back("[nix,a,nix,b,a,nix,a,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,b,b,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,b,nix,b,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,a,a,b,a]");
        solutions.push_back("[nix,a,nix,b,nix,a,nix,a,b,a]");
        solutions.push_back("[nix,a,nix,a,b,b,a,a]");
        solutions.push_back("[nix,a,nix,a,b,nix,b,a,a]");
        solutions.push_back("[nix,a,nix,a,a,a,a,a]");
        solutions.push_back("[nix,a,nix,a,a,nix,a,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,b,b,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,b,nix,b,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,a,a,a,a]");
        solutions.push_back("[nix,a,nix,a,nix,a,nix,a,a,a]");
        solutions.push_back("[b,b,b,b,b,b,b]");
        solutions.push_back("[b,b,b,a,b,b,b]");
        solutions.push_back("[b,b,b,nix,b,b,b,b]");
        solutions.push_back("[b,b,b,nix,a,b,b,b]");
        solutions.push_back("[b,b,a,b,a,b,b]");
        solutions.push_back("[b,b,a,a,a,b,b]");
        solutions.push_back("[b,b,a,nix,b,a,b,b]");
        solutions.push_back("[b,b,a,nix,a,a,b,b]");
        solutions.push_back("[b,b,nix,b,b,b,b,b]");
        solutions.push_back("[b,b,nix,b,a,b,b,b]");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());

        for (int i = 0; i < 25; i++) {
            answer = org.get_answer();
            REQUIRE(answer.has_value());
            for (auto &elem : *answer) {
                auto found = std::find(solutions.begin(), solutions.end(), elem.substitute);
                bool has_found = found != solutions.end();
                REQUIRE(has_found);
                solutions.erase(found);

            }
        }


    }

}



