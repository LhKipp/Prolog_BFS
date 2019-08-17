//
// Created by leonhard on 16.08.19.
//
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include <vector>
#include <map>
#include <string>

using  namespace std;
using  namespace wam;
TEST_CASE("BFS Organizer List Test"){
    std::vector<std::string> program_code;
    bfs_organizer org;
    auto setup_org = [&](string query) {
        org.load_program(program_code);
        org.load_query(query);
    };
    SECTION("Easy unification") {
        program_code.emplace_back("p([a,b],[a,b,c],[a,b,c,d]).");

        setup_org("p([A,B], [A,b,C], Z).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["B"] = "b";
        actual_substs["C"] = "c";
        actual_substs["Z"] = "[a,b,c,d]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 4);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }

    SECTION("Easy unification") {
        program_code.emplace_back("p([a,b],[a,b,c],[a,b,c,d]).");

        setup_org("p([A,B], [A,b,C], Z).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["A"] = "a";
        actual_substs["B"] = "b";
        actual_substs["C"] = "c";
        actual_substs["Z"] = "[a,b,c,d]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 4);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Easy Unification") {
        program_code.emplace_back("append([a | B]).");

        setup_org("append([a,b]).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "b";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 0);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Easy Unification") {
        program_code.emplace_back("append([a | R]).");

        setup_org("append([a,b,c]).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 0);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }

    SECTION("Append") {
        program_code.emplace_back("f([a,b]).");

        setup_org("f([a| B ])");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["B"] = "b";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("List with append operator") {
        program_code.emplace_back("f([a,b,c | d]).");

        setup_org("f(Z)");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "[a,b,c,d]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Append list properly coded - Empty list") {
        program_code.emplace_back("append([X| Xs], Ys, [X | Rs]) :- append(Xs, Ys, Rs).");
        program_code.emplace_back("append([], Xs, Xs).");

        setup_org("append(Z, [a], [a]).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "[]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Append list properly coded - Easy append") {
        program_code.emplace_back("append([X| Xs], Ys, [X | Rs]) :- append(Xs, Ys, Rs).");
        program_code.emplace_back("append([], Xs, Xs).");

        setup_org("append(Z, [c], [a,b,c]).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "[a,b]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Append list properly coded - Easy append 2") {
        program_code.emplace_back("append([X| Xs], Ys, [X | Rs]) :- append(Xs, Ys, Rs).");
        program_code.emplace_back("append([], Xs, Xs).");

        setup_org("append([a],Z, [a,b,c]).");

        auto found_answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "[b,c]";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
}
