//
// Created by leonhard on 24.05.20.
//

#include <catch2/catch.hpp>
#include "prolog_bfs/wam/data/heap_reg.h"

using namespace std;

TEST_CASE("Heap reg size test") {
    REQUIRE(sizeof(wam::heap_reg) == 8);
}




