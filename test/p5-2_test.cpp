// Created by leonhard on 18.08.19.
//
#include <iostream>
#include <catch2/catch.hpp>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p5-2 test") {
    bfs_organizer org;

    org.load_program_from_file("test_src/p5-2.pl");

    SECTION("[]") {
        org.load_query("lVonM([a,b]).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("[a]") {
        org.load_query("lVonM([b,a,b,a]).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(!answer.has_value());
    }
    SECTION("lVonM") {

        vector<string> solutions;
        solutions.push_back("[a,b]");
        solutions.push_back("[a,b,nix]");
        solutions.push_back("[a,a,b,b]");
        solutions.push_back("[a,a,b,b,nix]");
        solutions.push_back("[a,a,a,b,b,b]");
        solutions.push_back("[a,a,a,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,b,b,b,b]");
        solutions.push_back("[a,a,a,a,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,b,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b,nix]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b,b,nix]");
//        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b,b,b]");
        solutions.push_back("[a,a,a,a,a,a,a,a,a,a,a,a,a,b,b,b,b,b,b,b,b,b,b,b,b,b,nix]");
        org.load_query("lVonM(Z).");

        for (int i = 0; i < 25; i++) {
            auto answer = org.get_answer().get_answer();
            REQUIRE(answer.has_value());
            for (auto &elem : *answer) {
//                std::cout << elem<< endl;
                auto found = std::find(solutions.begin(), solutions.end(), elem.binding);
                bool has_found = found != solutions.end();
                REQUIRE(has_found);
                solutions.erase(found);
            }
        }
        REQUIRE(solutions.empty());


    }

}



