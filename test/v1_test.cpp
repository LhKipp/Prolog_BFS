//
// Created by leonhard on 19.08.19.
//
#include <catch2/catch.hpp>
#include <map>
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include "./test_util.h"

using namespace std;
using namespace wam;

TEST_CASE("v1 tests") {
    bfs_organizer org;

        vector<string> program_code;
        program_code.push_back("natSymb(o). %o ist ein natsymb");
        program_code.push_back("natSymb(s(A)) :- natSymb(A). %o ist ein natsymb");
        program_code.push_back("test(X) :- test(X).");
        program_code.push_back("test(a).");
        program_code.push_back("test(b).");

auto setup_org = [&](string query) {
    org.load_program(vec_to_string(program_code));
    org.load_query(query);
};
    SECTION("a") {
        setup_org("natSymb(s(A)).");
        auto ans = org.get_answer().get_answer();
        REQUIRE(ans.has_value());
        REQUIRE(ans->at(0).binding == "o");
    }
    SECTION("c") {
        setup_org("test(Z).");
        map<string,bool> found;
        auto ans = org.get_answer().get_answer();
        for(int i=0; i <5; i++){
                REQUIRE(ans.has_value());
                found[ans->at(0).binding ] = true;
                bool is_a_or_b = ans->at(0).binding == "a" || ans->at(0).binding == "b";
                REQUIRE(is_a_or_b);
                ans = org.get_answer().get_answer();
        }
        REQUIRE(found["a"]);
        REQUIRE(found["b"]);
    }

}



