//
// Created by leonhard on 05.08.19.
//
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "../src/wam/data/var_substitution.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace wam;

TEST_CASE("Practical 5 test") {
    std::vector<std::string> program_code;
    bfs_organizer org;

    program_code.push_back("zustand(readSigma).");
    program_code.push_back("zustand(dele).");
    program_code.push_back("sigma(a).");
    program_code.push_back("sigma(b).");
    program_code.push_back("kellerSym(raute).");
    program_code.push_back("kellerSym(a).");
    program_code.push_back("delta(readSigma, nix, raute, readSigma,nil).");
    program_code.push_back("delta(readSigma, a, raute, readSigma ,list(a,list(raute,nil))).");
    program_code.push_back("delta(readSigma, a, a, readSigma ,list(a,list(a,nil))).");
    program_code.push_back("delta(readSigma, nix, a, dele, list(a,nil)).");
    program_code.push_back("delta(readSigma, b, a, dele ,nil).");
    program_code.push_back("delta(dele, b, a, dele,nil).");
    program_code.push_back("delta(dele, nix, raute, dele ,nil).");
    program_code.push_back("startZ(readSigma).");
    program_code.push_back("startKeller(raute).");
    program_code.push_back("sigma_stern(nil).");
    program_code.push_back("sigma_stern(list(W, Rs) :- sigma(W), sigma_stern(Rs).");
    program_code.push_back("lappend(nil, Xs, Xs).");
    program_code.push_back("lappend(list(X, Rs), Xs, list(X, Ns)) :- lappend(Rs, Xs, Ns).");
    program_code.push_back(
            "es(Zustand, list(W, WRs) , list(G,GRs), ZNext, WRs, Stack_neu) :- delta(Zustand, W, G, ZNext, WriteBack), lappend(WriteBack, GRs, Stack_neu).");
    program_code.push_back(
            "es(Zustand, W, list(G, GRs), ZNext, W, Stack_neu) :- delta(Zustand, nix, G, ZNext, WriteBack), lappend(WriteBack, GRs, Stack_neu).");
    program_code.push_back("es_plus(Z0, W, G, Z1, WNeu, GNeu) :- es(Z0, W, G, Z1 , WNeu, GNeu).");
    program_code.push_back(
            "es_plus(Z0, W, G, ZNeu, WRs, Stack_neu) :- es(Z0, W, G, ZNext, WRest, Stack), es_plus(ZNext, WRest, Stack, ZNeu, WRs, Stack_neu).");
    program_code.push_back(
            "lVonM(Ws) :- startZ(Z0), zustand(EndZ), startKeller(K) , es_plus(Z0, Ws, list(K,nil), EndZ, nil, nil).");

    auto setup_org = [&](string query) {
        org.load_program(program_code);
        org.load_query(query);
    };

    SECTION("start") {
        setup_org("startZ(Z).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        map<std::string, std::string> actual_substs;
        actual_substs["Z"] = "readSigma";
        REQUIRE(answer.has_value());
        REQUIRE(answer->size() == 1);
        for (auto &subst : *answer) {
            REQUIRE(actual_substs.at(subst.var_name) == subst.substitute);
        }
    }
    SECTION("zustandEnd") {
        setup_org("zustand(Z).");

        map<std::string, int> actual_substs;
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        for (auto &subst : *answer) {
            actual_substs[subst.substitute]++;
        }
        auto next_answer = org.get_answer();
        REQUIRE(next_answer.has_value());
        for (auto &subst : *next_answer) {
            actual_substs[subst.substitute]++;
        }
        REQUIRE(actual_substs.at("readSigma") == 1);
        REQUIRE(actual_substs.at("dele") == 1);
    }
    SECTION("lappend") {
        setup_org("lappend(list(a,list(b,list(c,nil))),list(d,list(e,list(f,nil))),Z).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->at(0).substitute == "list(a,list(b,list(c,list(d,list(e,list(f,nil))))))");
    }
    SECTION("lappend") {
        setup_org("lappend(nil, list(a,list(b,nil)),Z).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->at(0).substitute == "list(a,list(b,nil))");
    }
    SECTION("lappend") {
        setup_org("lappend(list(a,list(b,nil)),nil, Z).");

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        REQUIRE(answer->at(0).substitute == "list(a,list(b,nil))");
    }
    SECTION("es") {
        setup_org("es(readSigma, list(a,nil), list(raute,nil), ZN, nil,list(a,list(raute,nil))).");
        map<std::string, int> actual_substs;
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        while (answer) {
            for (auto &subst : *answer) {
                actual_substs[subst.substitute]++;
            }
            answer = org.get_answer();
        }
        REQUIRE(actual_substs["readSigma"] == 1);
    }
    SECTION("es_plus") {
        setup_org("es_plus(readSigma, list(a,list(b,nil)), list(raute,nil), dele, nil,nil).");

        map<std::string, int> actual_substs;
        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        while (answer) {
            for (auto &subst : *answer) {
                actual_substs[subst.substitute]++;
                cout << subst << endl;
            }
            answer = org.get_answer();
        }
    }
    SECTION("lappend") {
        setup_org("lVonM(Z).");
        //Manually checked works

        auto answer = org.get_answer();
        REQUIRE(answer.has_value());
        while (answer) {
            for (auto &elem : *answer) {
                cout << elem << endl;
            }
//            answer = org.get_answer();
            break;
        }
    }

}



