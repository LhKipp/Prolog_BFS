//
// Created by leonhard on 30.07.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "../src/wam/data/var_substitution.h"
#include <map>
#include <string>
#include<iostream>

using namespace wam;
using namespace std;

TEST_CASE("BFS_Organizer Tests", "[BFS_Organizer]") {
    std::vector<std::string> program_code;
    bfs_organizer org;

    auto setup_org = [&](string query) {
        org.load_program(program_code);
        org.load_query(query);
    };

    SECTION("Query: p(Z,h(Z,W),f(W)) : Program: ") {
        program_code.emplace_back("p(f(X),h(Y,f(a)),Y)");

        setup_org("p(Z,h(Z,W),f(W))");

        auto answer = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["W"] = "f(a)";
        actual_substs["X"] = "f(a)";
        actual_substs["Y"] = "f(f(a))";
        actual_substs["Z"] = "f(f(a))";
        REQUIRE(answer.has_value());
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: f(g) : Program: f(X)") {
        program_code.emplace_back("f(X)");
        setup_org("f(g)");
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->empty());
    }
    SECTION("Query: r(x,h(Z,W),f(g)) : Program: ") {
        program_code.emplace_back("r(Z,h(Z,f(a)),f(Z)");
        //Z/x, W/f(a) , Z/g
        setup_org("r(x,h(Z,W),f(g))");

        auto found_answer = org.get_answer();
        REQUIRE(!found_answer);
    }
    SECTION("Query: r(x,x) : Program: ") {
        program_code.emplace_back("r(x)");

        setup_org("r(x,x)");

        auto found_answer = org.get_answer();
        REQUIRE(!found_answer);
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(x)");

        setup_org("r(f(X),h(a,b),x)");

        auto found_answer = org.get_answer();
        REQUIRE(!found_answer);
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(h(A,B),f(B),h(A,C))");

        setup_org("r(h(a,Z),f(b),h(a,c))");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "b";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            cout<< subst;
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(h(A,B),f(B),h(A,C),f(f(D)),E)");

        setup_org("r(h(a,Z),f(b),h(a,c),f(f(d)),e)");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "b";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            cout<< subst;
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(f(a),h(X,Z),J)");

        setup_org("r(f(X),h(a,b),x)");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            cout<< subst;
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(f(X),h(a,Z),J)");

        setup_org("r(f(a),h(X,b),x)");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            cout<< subst;
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(f(a),h(X,Z),J)");

        setup_org("r(f(a),h(a,b),x)");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        REQUIRE(found_answer.has_value());
        REQUIRE(found_answer->empty());
    }
    SECTION("Query: r(f(f(f(X))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(f(f(a))),h(f(f(f(f(b)))),b))");

        setup_org("r(f(f(f(X))),h(f(f(f(f(Z)))),b))");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        actual_substs["Z"] = "b";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(h(a,f(f(b)))),h(f(f(f(f(b)))),b))");

        setup_org("r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),Y))");

        auto found_answer = org.get_answer();
        REQUIRE(found_answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["X"] = "a";
        actual_substs["Z"] = "b";
        actual_substs["Y"] = "b";
        REQUIRE(found_answer.has_value());
        for (auto &subst : *found_answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: r(f(h(X,f(f(Z)))),h(f(f(f(f(Z)))),b)) : Program: ") {
        program_code.emplace_back("r(f(h(a,f(f(b)))),h(f(f(f(f(b)))),b))");

        setup_org("r(f(h(X,f(Z))),h(f(f(f(f(Z)))),X))");

        auto found_answer = org.get_answer();
        REQUIRE(!found_answer);
    }
}
