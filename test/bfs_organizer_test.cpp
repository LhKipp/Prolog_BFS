//
// Created by leonhard on 30.07.19.
//
#include "../catch.hpp"

#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>
using namespace wam;
using namespace std;
TEST_CASE("BFS_Organizer Tests", "[BFS_Organizer]"){
    SECTION("Query: p(Z,h(Z,W),f(W)) : Program: "){
        bfs_organizer org;
        std::vector<std::string> program_code;
        program_code.emplace_back("p(f(X),h(Y,f(a)),Y)");
        org.load_program(program_code);

        org.load_query("p(Z,h(Z,W),f(W))");

        bool found_answer;
        var_substitutions substitutions;
        std::tie(substitutions, found_answer) = org.get_answer();

        map<std::string, std::string> actual_substs;
        actual_substs["W"] = "f(a)";
        actual_substs["X"] = "f(a)";
        actual_substs["Y"] = "f(f(a))";
        actual_substs["Z"] = "f(f(a))";
        REQUIRE(found_answer);
//        for(substitutions)
    }
}
//TEST_CASE("Parser program instructions test", "[Parser]"){
//    using tag = wam::heap_tag ;
//    wam::executor exe;
//    auto& heap = exe.heap;
//
//    auto instr = wam::parse_query( "p(Z,h(Z,W),f(W))");
//
//    for(auto& instruct : std::get<0>(instr).instructions){instruct(exe);}
//
//
//    auto instr_prog = wam::parse_program_term("");
//
//    for(auto& instruct : instr_prog.instructions){
//        instruct(exe);
//    }
//
//    std::cout <<"hello";
//}

//TEST_CASE("Parser query instructions test", "[Parser]"){
//    bfs_or
//    using tag = wam::heap_tag ;
//    wam::executor exe;
//    auto instr = wam::parse_query("");
//
//    for(auto& instruct : std::get<0>(instr).instructions){instruct(exe);}
//
//    auto heap = exe.heap;
//    REQUIRE(heap[0].type == tag::STR);
//    REQUIRE(heap[1].type == tag::FUN);
//    REQUIRE(heap[2].type == tag::REF);
//
//    REQUIRE(heap[3].type == tag::STR);
//    REQUIRE(heap[4].type == tag::FUN);
//    REQUIRE(heap[5].type == tag::REF);
//    REQUIRE(heap[6].type == tag::REF);
//
//    REQUIRE(heap[7].type == tag::STR);
//    REQUIRE(heap[8].type == tag::FUN);
//    REQUIRE(heap[9].type == tag::REF);
//    REQUIRE(heap[10].type == tag::STR);
//    REQUIRE(heap[11].type == tag::STR);
//}
