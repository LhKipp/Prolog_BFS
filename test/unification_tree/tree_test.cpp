//
// Created by leonhard on 08.03.20.
//
#include "../../catch.hpp"
#include "../../src/wam/bfs_organizer/bfs_organizer.h"
#include <map>
#include <string>
#include <iostream>

using namespace wam;
using namespace std;

TEST_CASE("Tree mult") {
    bfs_organizer org;

    org.load_program_from_file("test_src/arithmetic.pl");

    org.load_query("mult(s(o),s(s(s(o))),Z).");

    auto ans = org.get_answer();
    query_node t = org.get_unification_tree();


}


