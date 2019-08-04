//
// Created by leonhard on 03.08.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "../src/wam/data/var_substitution.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("BFS_Organizer L2 Tests") {
    std::vector<std::string> program_code;
    program_code.push_back("p(X,Y) :- q(X,Z), r(Z,Y).");
    program_code.push_back("q(x,z).");
    program_code.push_back("r(z,y).");
    program_code.push_back("a(A,B) :- b(B,C), c(C,D), d(D,E), e(E,F), f(F,A).");
    program_code.push_back("b(b,c).");
    program_code.push_back("c(c,d).");
    program_code.push_back("d(d,e).");
    program_code.push_back("e(e,f).");
    program_code.push_back("f(f,a).");
    bfs_organizer org;

    auto setup_org = [&](string query) {
        org.load_program(program_code);
        org.load_query(query);
    };
    SECTION("Easy Chain"){
        setup_org("p(X,Y)");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "x";
        actual_substs["Y"] = "y";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Deep easy Chain"){
        setup_org("a(A,B).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["B"] = "b";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Deep easy Chain"){
        setup_org("a(A, B).");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["B"] = "b";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
}
