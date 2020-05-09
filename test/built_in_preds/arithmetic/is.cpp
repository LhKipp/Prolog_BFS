#include "../../../catch.hpp"
#include <wam/bfs_organizer/bfs_organizer.h>
#include <string>



using namespace wam;
using namespace std;

void make_is_test_case(std::string rhs, int rhs_val){
    bfs_organizer org;
    org.load_query("X is " + rhs);
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == std::to_string(rhs_val));

    org.load_query(to_string(rhs_val) + " is " + rhs);
    auto ans2 = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
}

TEST_CASE("Is correct assignment") {
    make_is_test_case("3.", 3);
    make_is_test_case("3 + 3.", 6);
    make_is_test_case("3 - 4.", -1);
    make_is_test_case("3 * 3.", 9);
    make_is_test_case("9 // 3.", 3);
    make_is_test_case("2^3.", 8);
    make_is_test_case("3 * 3 - 1.", 8);
    make_is_test_case("(4 + 4) * 4 - ((2 + 3)*2).", 22);
    make_is_test_case("2 ^ 3 * 4.", 32);
    make_is_test_case("(((3^2)-1)*1+2) - 10.", 0);
}

TEST_CASE("Variable is assigment"){
    bfs_organizer org;
    org.load_query("X is (4 + 4) * 4 - ((2 + 3)*2).");
    auto ans = org.get_answer().get_answer();
    REQUIRE(ans.has_value());
    REQUIRE(ans->at(0).binding == std::to_string(22));
}

TEST_CASE("is comparison rules"){
    bfs_organizer org;
    //THIS IS THE BEHAVIOR OF SWIPL AND NOT NECESSARILY THE OPINION OF THE AUTHOR
    auto ass_false = [&](auto q){
        org.load_query(q);
        auto ans = org.get_answer().get_answer();
        REQUIRE(!ans.has_value());
    };
    ass_false("[] is 3.");
    ass_false("a is 3.");
    ass_false("f(a) is 3.");
}
