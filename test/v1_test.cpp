//
// Created by leonhard on 19.08.19.
//
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("v1 tests") {
    bfs_organizer org;

        vector<string> code;
        code.push_back("natSymb(o). %o ist ein natsymb");
        code.push_back("natSymb(s(A)) :- natSymb(A). %o ist ein natsymb");

    SECTION("a") {
        org.load_program(code);
        org.load_query("natSymb(s(A)).");
        auto ans = org.get_answer();
        REQUIRE(ans.has_value());
        REQUIRE(ans->at(0).substitute == "o");
    }

}



