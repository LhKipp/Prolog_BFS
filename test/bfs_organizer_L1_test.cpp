//
// Created by leonhard on 30.07.19.
//
#include <catch2/catch.hpp>

#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"
#include "prolog_bfs/wam/data/var_binding.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("BFS_Organizer_Tests", "[L1]") {
    std::vector<std::string> program_code;
    bfs_organizer org;

    auto setup_org = [&](string query) {
        auto code = std::accumulate(program_code.begin(), program_code.end(),
                std::string());
        org.load_program(code);
        org.load_query(query);
    };

    SECTION("Query: a.: Program: a.") {
        program_code.emplace_back("a.");

        setup_org("a.");
        REQUIRE(true);

        auto found_answer = org.get_answer().get_answer();

        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 0);
    }
    SECTION("Query: b.: Program: a.") {
        program_code.emplace_back("a.");

        setup_org("b.");

        auto found_answer = org.get_answer().get_answer();

        REQUIRE(!found_answer.has_value());
        /*The test below fails if compiled with clang. Passes if compiled with gcc*/
        //REQUIRE(found_answer->size() == 0);
    }
    SECTION("Query: p(Z,h(Z,W),f(W)) : Program: ") {
        program_code.emplace_back("p(f(X),h(Y,f(a)),Y).");

        setup_org("p(Z,h(Z,W),f(W)).");

        auto found_answer = org.get_answer().get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["W"] = "f(a)";
        actual_substs["X"] = "f(a)";
        actual_substs["Y"] = "f(f(a))";
        actual_substs["Z"] = "f(f(a))";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 2);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Query: f(g) : Program: f(X)") {
        program_code.emplace_back("f(X).");

        setup_org("f(g).");

        auto found_answer = org.get_answer().get_answer();

        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->empty());
    }

    SECTION("Query: f(g) : Program: f(Lhs, Rhs)") {
        program_code.emplace_back("f(Lhs, Rhs).");

        setup_org("f(a, b).");

        auto found_answer = org.get_answer().get_answer();

        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->empty());
    }
    SECTION("Query: r(x,h(Z,W),f(g)) : Program: ") {
        program_code.emplace_back("r(Z,h(Z,f(a)),f(Z)).");
        //Z/x, W/f(a) , Z/g
        setup_org("r(x,h(Z,W),f(g)).");

        auto found_answer = org.get_answer().get_answer();
        REQUIRE(!(found_answer.has_value()));
    }
    SECTION("Query: r(x,x) : Program: ") {
        program_code.emplace_back("r(x).");

        setup_org("r(x,x).");

        auto found_answer = org.get_answer().get_answer();
        REQUIRE(!(found_answer.has_value()));
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(x).");

        setup_org("r(f(X),h(a,b),x).");

        auto found_answer = org.get_answer().get_answer();
        REQUIRE(!(found_answer.has_value()));
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(f(a),h(D,Z)).");

        setup_org("r(f(X),h(a,b)).");
        auto found_answer = org.get_answer().get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(f(a),h(D,Z),J).");

        setup_org("r(f(X),h(a,b),x).");
        auto found_answer = org.get_answer().get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 1);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Query: r(f(f(f(X))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(f(f(a))),h(f(f(f(f(b)))),b)).");

        setup_org("r(f(f(f(X))),h(f(f(f(f(Z)))),b)).");
        auto found_answer = org.get_answer().get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        actual_substs["Z"] = "b";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Query: r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(h(a,f(f(b)))),h(f(f(f(f(b)))),b)).");

        setup_org("r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),Y)).");



        auto found_answer = org.get_answer().get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        actual_substs["Z"] = "b";
        actual_substs["Y"] = "b";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->size() == 3);
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        }
    }
    SECTION("Query: r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(h(a,f(f(b)))),h(f(f(f(f(b)))),b)).");

        setup_org("r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),X)).");

        auto found_answer = org.get_answer().get_answer();
        REQUIRE(!found_answer.has_value());
    }
}
