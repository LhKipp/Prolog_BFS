//
// Created by leonhard on 08.03.20.
//
#include "../../catch.hpp"
#include "../../src/wam/bfs_organizer/bfs_organizer.h"
#include <string>
#include <iostream>

using namespace wam;
using namespace std;

TEST_CASE("Tree mult") {
    bfs_organizer org;

    org.load_program_from_file("test_src/arithmetic.pl");

    org.load_query("mult(s(o),s(s(s(o))),Z).");

    auto ans = org.get_answer().get_answer();
    query_node t = org.get_unification_tree();

    REQUIRE(t.get_resolved_query_as_str() == "mult(s(o), s(s(s(o))), Z)");
    REQUIRE(!t.failed());

    std::vector<var_binding_node>& facts = t.get_children();
    assert(facts.size() == 2);
    assert(facts[0].get_fact_as_str() == "mult(o, X, o)");
    assert(facts[0].failed());
    assert(!facts[0].succeeded());
    assert(!facts[0].continues());

    var_binding_node& continuation = facts[1];
    assert(continuation.continues());
    assert(continuation.get_fact_as_str() == "mult(s(A), B, C)");
    auto& bindings = continuation.get_var_bindings();

    vector<var_binding> solutions;
    solutions.emplace_back("B","s(s(s(o)))");
    solutions.emplace_back("C","Z");
    solutions.emplace_back("A","o");
    for (auto &elem : bindings) {
//        std::cout << elem<< endl;
        auto found = std::find(solutions.begin(), solutions.end(), elem);
        bool has_found = found != solutions.end();
        REQUIRE(has_found);
        solutions.erase(found);
    }
    REQUIRE(!continuation.get_var_bindings_as_str().empty());
    REQUIRE(solutions.empty());

    const auto& q2 = continuation.get_continuing_query();
    REQUIRE(!q2.failed());
    REQUIRE(q2.get_resolved_query_as_str() == "mult(o, s(s(s(o))), D)");
    REQUIRE(q2.get_children().size() == 2);

    auto& failed_2 = q2.get_children()[1];
    REQUIRE(failed_2.failed());
    REQUIRE(failed_2.get_fact_as_str() == "mult(s(A), B, C)");

    auto& mult_fact = q2.get_children()[0];
    REQUIRE(mult_fact.get_fact_as_str() == "mult(o, X, o)");
    REQUIRE(mult_fact.continues());
    REQUIRE(mult_fact.get_atom().get_belonging_rule()->atoms().size() == 1);

    const auto& bindings2 = mult_fact.get_var_bindings();
    solutions.emplace_back("D","o" );
    solutions.emplace_back("X","s(s(s(o)))");
    for (const auto &elem : bindings2) {
        auto found = std::find(solutions.begin(), solutions.end(), elem);
        bool has_found = found != solutions.end();
        REQUIRE(has_found);
        solutions.erase(found);
    }
    REQUIRE(solutions.empty());

    auto & addQuery = mult_fact.get_continuing_query();
    REQUIRE(!addQuery.failed());
    REQUIRE(addQuery.get_children().size() == 2);

    auto& addRuleFail = addQuery.get_children()[1];
    REQUIRE(addRuleFail.is_to_be_continued());
    REQUIRE(addRuleFail.get_fact_as_str() == "add(s(A), B, C)");

    auto& succeededAddFact = addQuery.get_children()[0];
    REQUIRE(succeededAddFact.succeeded());

    const auto& bindings3 = succeededAddFact.get_var_bindings();
    solutions.emplace_back("A","s(s(s(o)))" );
    solutions.emplace_back("Z","s(s(s(o)))");
    for (const auto &elem : bindings3) {
        auto found = std::find(solutions.begin(), solutions.end(), elem);
        bool has_found = found != solutions.end();
        REQUIRE(has_found);
        solutions.erase(found);
    }
    REQUIRE(solutions.empty());

    auto& finalBindings = succeededAddFact.get_final_var_bindings();
    REQUIRE(finalBindings.size() == 1);
    solutions.emplace_back("Z","s(s(s(o)))");
    auto found = std::find(solutions.begin(), solutions.end(), finalBindings[0]);
    bool has_found = found != solutions.end();
    REQUIRE(has_found);
}



TEST_CASE("Tree additional var"){
    bfs_organizer org;
    std::string code =
            "p(X) :- addVar(X), unifyVar(X)."
            "addVar(s(Y))."
            "unifyVar(s(s(o))).";
    org.load_program(code);
    org.load_query("p(Y).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());

    var_binding expected{"Y", "s(s(o))"};
    REQUIRE((*ans)[0] == expected);
    query_node t = org.get_unification_tree();
    auto& childr = t.get_children();
    var_binding_node& pXChild = childr.at(0);
    expected = {"X", "Y"};
    REQUIRE(pXChild.get_var_bindings().at(0) == expected);
    query_node& addVarX = pXChild.get_continuing_query();
    REQUIRE(addVarX.get_resolved_query_as_str() == "addVar(Y)");
    var_binding_node& addVarSY = addVarX.get_children().at(0);
    REQUIRE(addVarSY.get_var_bindings()[0] == var_binding{"Y", "s(Y)"});
    REQUIRE(addVarSY.get_var_bindings().size() == 1);
    query_node& unifyVar = addVarSY.get_continuing_query();
    REQUIRE(unifyVar.get_resolved_query_as_str() == "unifyVar(s(Y))");
    var_binding_node& unifyVarSSO = unifyVar.get_children()[0];
    REQUIRE(unifyVarSSO.get_var_bindings()[0] == var_binding{"Y", "s(o)"});
}

TEST_CASE("Tree to be continued node"){
    std::string code =
            "p(X) :- p(X)."
            "p(a).";
    bfs_organizer org;
    org.load_program(code);
    org.load_query("p(Z).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    query_node t = org.get_unification_tree();

    REQUIRE(!t.failed());
    auto& childr = t.get_children();
    auto& pXChild = childr[0];
    REQUIRE(pXChild.continues());
    std::vector<var_binding> substitutions;
    substitutions.emplace_back("X", "Z");
    REQUIRE(pXChild.get_var_bindings()[0] == substitutions[0]);
    auto & toBeCont = pXChild.get_continuing_query();
    //Cant get the name of a to_be_continued query_node
//    REQUIRE(toBeCont.get_resolved_query_as_str() == "p(Z)");
    REQUIRE(toBeCont.is_to_be_continued());

    auto& finalChild = childr[1];
    substitutions.clear();
    substitutions.emplace_back("Z", "a");
    REQUIRE(finalChild.succeeded());
    REQUIRE(finalChild.get_final_var_bindings()[0] == substitutions[0]);

}

TEST_CASE("Tree fuzzing"){
    bfs_organizer org;
    org.load_program_from_file("test_src/p5-2.pl");
    org.load_query("lVonM(Z).");

    for (int i = 0; i < 5; ++i) {
        auto ans = org.get_answer().get_answer();
        org.get_unification_tree();
    }

    org.load_program_from_file("test_src/p4-0.pl");
    org.load_query("lvonN(Z).");
    org.get_answer().get_answer();
    org.get_unification_tree();
    org.get_answer().get_answer();
    org.get_unification_tree();
    org.get_answer().get_answer();
    org.get_unification_tree();
    org.get_answer().get_answer();
    org.get_unification_tree();
    //require not failed
}

TEST_CASE("Tree with list unifications"){
    std::string code = "r([a, b, c]) :- g(X), h(Y), c(Z), f(Single)."
                       "g([[a,b], [a,Y] | X])."
                       "h([])."
                       "c([X, Y])."
                       "f([a]).";

    bfs_organizer org;
    org.load_program(code);
    org.load_query("r(Z).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    query_node t = org.get_unification_tree();
    var_binding_node& r = t.get_children()[0];
    REQUIRE(r.get_var_bindings()[0] == var_binding{"Z", "[a, b, c]"});
    query_node& gX = r.get_continuing_query();
    var_binding_node& g = gX.get_children()[0];
    REQUIRE(g.get_var_bindings()[0] == var_binding{"X", "[[a, b], [a, Y], X]"});
    query_node& hY = g.get_continuing_query();
    var_binding_node& h = hY.get_children()[0];
    REQUIRE(h.get_var_bindings()[0] == var_binding{"Y", "[]"});
    query_node& cZ = h.get_continuing_query();
    var_binding_node& c = cZ.get_children()[0];
    REQUIRE(c.get_var_bindings()[0] == var_binding{"Z", "[X, Y]"});
    query_node& single = c.get_continuing_query();
    var_binding_node& fSingle = single.get_children()[0];
    REQUIRE(fSingle.get_var_bindings()[0] == var_binding{"Single", "[a]"});
}

TEST_CASE("Tree add not failing"){
    bfs_organizer org;
    org.load_program_from_file("test_src/arithmetic.pl");
    org.load_query("add(Z, s(o), s(s(o))).");

    org.get_answer().get_answer();
    org.get_unification_tree();
}

TEST_CASE("Tree test predicate not in program"){
    bfs_organizer org;
    org.load_program(
            "weiblich(e2)."
            "weiblich(k2)."
            "schwester(X,Y) :- weiblich(X), ges(X,Y).");
    org.load_query("schwester(X, Y).");
    org.get_answer().get_answer();
    auto t = org.get_unification_tree();
    REQUIRE(true);
}
