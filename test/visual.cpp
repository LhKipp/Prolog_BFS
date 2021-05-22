//
// Created by leonhard on 10.05.20.
//

#include "../catch.hpp"
#include <map>
#include "../src/wam/bfs_organizer/bfs_organizer.h"
#include "test_util.h"

using namespace std;
using namespace wam;

//These tests are not working yet

// Currently disabled as it doesnt work
TEST_CASE("Visual: Vars don't have index appended") {
    bfs_organizer org;

    const char * prog =
            "g(X, X).";
    org.load_program(prog);
    org.load_query("g(Z1, Z2).");
    auto ans = org.get_answer().get_answer();
    org.get_unification_tree();
    has_all_of_these(ans, {
            "Z1", "Z2",
            "Z2", "Z2"
    });
    org.get_unification_tree();
}
