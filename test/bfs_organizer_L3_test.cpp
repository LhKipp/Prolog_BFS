//
// Created by leonhard on 04.08.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "../src/wam/data/var_substitution.h"
#include <map>
#include <string>

using namespace wam;
using namespace std;

TEST_CASE("BFS_Organizer L3 Tests") {
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
    program_code.push_back("g(a,f(h(f(d),a,b))) :- c(c,d), a(A,B), p(X,Y),e(E,F).");
    program_code.push_back("g(a,f(b)).");
    bfs_organizer org;

auto setup_org = [&](string query) {
    auto code = std::accumulate(program_code.begin(), program_code.end(),
                                std::string());
    org.load_program(code);
    org.load_query(query);
};

    SECTION("Easy endless loop"){
        program_code.push_back("z(X) :- z(X).");
        program_code.push_back("z(a).");
        setup_org("z(A).");

        auto answer = org.get_answer();
        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 1);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Easy endless loop"){
        program_code.push_back("z(X) :- z(X).");
        program_code.push_back("z(a).");
        setup_org("z(A).");

        auto answer = org.get_answer();
        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 1);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("hard with endless loop"){
        program_code.push_back("z(X) :- z(X).");
        program_code.push_back("z(a).");
        setup_org("z(A), p(X,y), z(A), a(A,b).");

        auto answer = org.get_answer();
        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["X"] = "x";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 2);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Easy branch"){
        program_code.push_back("x(A) :- a(A,B).");
        program_code.push_back("x(A) :- a(a,A).");
        setup_org("x(A).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        map<std::string, int> actual_substs;
        int total_answers=0;
        while(answer) {
            total_answers += answer->size();
            for (auto &subst : *answer) {
                actual_substs[subst.substitute]+= 1;
            }
            answer = org.get_answer();
        }
        REQUIRE(total_answers == 2);
        REQUIRE(actual_substs.size() ==2);
        REQUIRE(actual_substs["a"] ==1);
        REQUIRE(actual_substs["b"] ==1);
    }

    SECTION("hard branch"){
        program_code.push_back("x(A) :- p(x,Y),a(A,B),c(C,d).");
        program_code.push_back("x(A) :- h(X,Y,Z), p(D,J), a(a,A).");
        setup_org("x(A).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        map<std::string, int> actual_substs;
        int total_answers=0;
        while(answer) {
            total_answers += answer->size();
            for (auto &subst : *answer) {
                actual_substs[subst.substitute]+= 1;
            }
            answer = org.get_answer();
        }
        REQUIRE(total_answers == 2);
        REQUIRE(actual_substs.size() ==2);
        REQUIRE(actual_substs["a"] ==1);
        REQUIRE(actual_substs["b"] ==1);
    }

    SECTION("Easy branch, multiple queries"){
        program_code.push_back("z(X) :- z(X).");
        program_code.push_back("z(a).");
        program_code.push_back("x(A) :- a(A,B).");
        program_code.push_back("x(B) :- a(a,B).");
        setup_org("x(A), z(A), g(A,B).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        map<std::string, int> actual_substs;
        int total_answers=0;
        while(answer) {
            total_answers += answer->size();
            for (auto &subst : *answer) {
                actual_substs[subst.substitute]+= 1;
            }
            answer = org.get_answer();
            if(total_answers > 40) break;
        }
        REQUIRE(actual_substs.size() ==3);
        REQUIRE(actual_substs["a"] >1);
        REQUIRE(actual_substs["f(b)"] >1);
        REQUIRE(actual_substs["f(h(f(d),a,b))"] >1);
    }
}
