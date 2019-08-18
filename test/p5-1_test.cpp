//
// Created by leonhard on 18.08.19.
//
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("p5-1 test") {
    bfs_organizer org;

    org.load_program("test_src/p5-1.pl");

    SECTION("[]") {
        org.load_query("lVonM(list(nix,nil)).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
    }
    SECTION("[a]") {
        org.load_query("lVonM( list(a,nil)).");
        auto answer = org.get_answer();
        REQUIRE(!answer.has_value());
    }
    SECTION("lVonM") {

        vector<string> solutions;
        solutions.push_back("list(nix,nil)");
        solutions.push_back("list(a,list(b,nil))");
        solutions.push_back("list(a,list(b,list(nix,nil)))");
        solutions.push_back("list(a,list(b,nil))");
        solutions.push_back("list(a,list(b,list(nix,nil)))");
        solutions.push_back("list(a,list(nix,list(b,nil)))");
        solutions.push_back("list(a,list(nix,list(b,list(nix,nil))))");
        solutions.push_back("list(a,list(a,list(b,list(b,nil))))");
        solutions.push_back("list(a,list(a,list(b,list(b,list(nix,nil)))))");
        solutions.push_back("list(a,list(a,list(b,list(b,nil))))");
        solutions.push_back("list(a,list(a,list(b,list(b,list(nix,nil)))))");
        solutions.push_back("list(a,list(a,list(nix,list(b,list(b,nil)))))");
        solutions.push_back("list(a,list(a,list(nix,list(b,list(b,list(nix,nil))))))");
        solutions.push_back("list(a,list(a,list(a,list(b,list(b,list(b,nil))))))");
        solutions.push_back("list(a,list(a,list(a,list(b,list(b,list(b,list(nix,nil)))))))");
        solutions.push_back("list(a,list(a,list(a,list(b,list(b,list(b,nil))))))");
        solutions.push_back("list(a,list(a,list(a,list(b,list(b,list(b,list(nix,nil)))))))");
        solutions.push_back("list(a,list(a,list(a,list(nix,list(b,list(b,list(b,nil)))))))");
        solutions.push_back("list(a,list(a,list(a,list(nix,list(b,list(b,list(b,list(nix,nil))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(b,list(b,list(b,list(b,nil))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(b,list(b,list(b,list(b,list(nix,nil)))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(b,list(b,list(b,list(b,nil))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(b,list(b,list(b,list(b,list(nix,nil)))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(nix,list(b,list(b,list(b,list(b,nil)))))))))");
        solutions.push_back("list(a,list(a,list(a,list(a,list(nix,list(b,list(b,list(b,list(b,list(nix,nil))))))))))");

        org.load_query("lVonM(Z).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());

        for (int i = 0; i < 25; i++) {
            answer = org.get_answer();
            REQUIRE(answer.has_value());
            for (auto &elem : *answer) {
//                std::cout << elem<< endl;
                 auto found = std::find(solutions.begin(), solutions.end(), elem.substitute);
                 bool has_found = found != solutions.end();
                 REQUIRE(has_found);
                 solutions.erase(found);
            }
        }
        REQUIRE(solutions.empty());


    }

}



