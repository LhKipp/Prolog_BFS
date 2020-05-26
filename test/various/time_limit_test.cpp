//
// Created by leonhard on 23.05.20.
//

#include "../../catch.hpp"
#include <iostream>
#include <map>
#include "../src/wam/bfs_organizer/bfs_organizer.h"

using namespace std;
using namespace wam;

TEST_CASE("Out of time tests") {
    bfs_organizer org;

    org.load_program_from_file("test_src/p5-2.pl");

    SECTION("a") {
        org.set_time_limit(std::chrono::microseconds{0});
        org.load_query("lVonM([a,a,a,a,b,b,b,b]).");
        auto ans = org.get_answer();
        REQUIRE(ans.is_error());
        auto err = ans.get_error();
        REQUIRE(err.err_code == ERROR_TYPE::OUT_OF_TIME);
    }
}



