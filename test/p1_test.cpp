//
// Created by leonhard on 28.04.20.
//
#include "../catch.hpp"
#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include <string>

using namespace wam;
TEST_CASE("Tree eq") {
    wam::bfs_organizer org;
    org.load_program_from_file("test_src/p1.pl");
    org.load_query("schwester(X, Y).");
    org.get_answer();
    auto t = org.get_unification_tree();
}

