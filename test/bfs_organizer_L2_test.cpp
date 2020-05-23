//
// Created by leonhard on 03.08.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "../src/wam/data/var_binding.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("BFS_Organizer_L2_Tests") {
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
    program_code.push_back("h(g(f(A),X),f(b),f(g(h(X,Y,B),c))) :- c(c,d), a(A,B), p(X,Y),e(E,F).");
    program_code.push_back("a.");
    program_code.push_back("b.");
    program_code.push_back("c.");
    program_code.push_back("d.");
    bfs_organizer org;

auto setup_org = [&](string query) {
    auto code = std::accumulate(program_code.begin(), program_code.end(),
                                std::string());
    org.load_program(code);
    org.load_query(query);
};
    SECTION("Easy Chain"){
        setup_org("p(X,Y).");

        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "x";
        actual_substs["Y"] = "y";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Deep easy Chain"){
        setup_org("a(A,B).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["B"] = "b";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Deep hard Chain"){
//        program_code.push_back("h(g(f(A),X),f(b),f(g(h(X,Y,B),c))) :- c(c,d), a(A,B), p(X,Y),e(E,F).");
        setup_org("h(g(f(A),X),f(b),f(F)).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        actual_substs["F"] = "g(h(x,y,b),c)";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 3);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Multiple Queries in once"){
//        program_code.push_back("h(g(f(A),X),f(b),f(g(h(X,Y,B),c))) :- c(c,d), a(A,B), p(X,Y),e(E,F).");
        setup_org("h(g(f(A),X),f(b),f(F)), p(X,Y), a(A,B).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        actual_substs["B"] = "b";
        actual_substs["Y"] = "y";
        actual_substs["F"] = "g(h(x,y,b),c)";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 5);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Many Queries in once"){
        setup_org("h(g(f(A),X),f(b),f(F)), p(X,Y), a(A,B),p(X,Y), a(A,B).");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        actual_substs["B"] = "b";
        actual_substs["Y"] = "y";
        actual_substs["F"] = "g(h(x,y,b),c)";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 5);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Many Queries in once with constants"){
        setup_org("h(g(f(A),X),f(b),f(F)), a, p(X,Y),b, a(A,B),p(X,Y), c,a(A,B),d.");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        actual_substs["B"] = "b";
        actual_substs["Y"] = "y";
        actual_substs["F"] = "g(h(x,y,b),c)";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 5);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Many Queries in once with constants"){
        setup_org("a,d ,h(g(f(A),X),f(b),f(F)), a, p(X,Y),b, a(A,B),p(X,Y), c,a(A,B),d.");
        auto answer = org.get_answer().get_answer();
        REQUIRE(answer.has_value());

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        actual_substs["B"] = "b";
        actual_substs["Y"] = "y";
        actual_substs["F"] = "g(h(x,y,b),c)";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 5);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Many Queries in once with constants"){
        setup_org("a,d ,h(g(f(A),X),f(b),f(F)), a,fault, p(X,Y),b, a(A,B),p(X,Y), c,a(A,B),d.");
        auto answer = org.get_answer().get_answer();
        REQUIRE(!answer.has_value());
    }
}
