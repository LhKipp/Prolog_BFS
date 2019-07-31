//
// Created by leonhard on 30.07.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>

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

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["W"] = "f(a)";
        actual_substs["X"] = "f(a)";
        actual_substs["Y"] = "f(f(a))";
        actual_substs["Z"] = "f(f(a))";
        REQUIRE(found_answer);
        for (auto &subst : substitutions) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("Query: f(g) : Program: f(X)") {
        program_code.emplace_back("f(X)");

        setup_org("f(g)");

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();

        REQUIRE(found_answer);
        REQUIRE(substitutions.size() == 0);
    }
    SECTION("Query: r(x,h(Z,W),f(g)) : Program: ") {
        program_code.emplace_back("r(Z,h(Z,f(a)),f(Z)");

        setup_org("r(x,h(Z,W),f(g))");

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();
        REQUIRE(!found_answer);
    }
    SECTION("Query: r(x,x) : Program: ") {
        program_code.emplace_back("r(x)");

        setup_org("r(x,x)");

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();
        REQUIRE(!found_answer);
    }
    SECTION("Query: r(f(X),h(a,b),x) : Program: ") {
        program_code.emplace_back("r(x)");

        setup_org("r(f(X),h(a,b),x)");

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();
        REQUIRE(!found_answer);
    }
}
