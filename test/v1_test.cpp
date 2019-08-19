//
// Created by leonhard on 19.08.19.
//
#include "../catch.hpp"
#include <iostream>
#include <map>
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("v1 tests") {
    bfs_organizer org;

        vector<string> code;
        code.push_back("natSymb(o). %o ist ein natsymb");
        code.push_back("natSymb(s(A)) :- natSymb(A). %o ist ein natsymb");
        code.push_back("test(X) :- test(X).");
        code.push_back("test(a).");
        code.push_back("test(b).");

    SECTION("a") {
        org.load_program(code);
        org.load_query("natSymb(s(A)).");
        auto ans = org.get_answer();
        REQUIRE(ans.has_value());
        REQUIRE(ans->at(0).substitute == "o");
    }
    SECTION("c") {
        org.load_program(code);
        org.load_query("test(Z)");
        map<string,bool> found;
        auto ans = org.get_answer();
        for(int i=0; i <5; i++){
                REQUIRE(ans.has_value());
                found[ans->at(0).substitute ] = true;
                bool is_a_or_b = ans->at(0).substitute == "a" || ans->at(0).substitute == "b";
                REQUIRE(is_a_or_b);
                ans = org.get_answer();
        }
        REQUIRE(found["a"]);
        REQUIRE(found["b"]);
    }

}



