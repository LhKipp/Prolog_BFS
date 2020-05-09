//
// Created by leonhard on 09.05.20.
//

#include "../catch.hpp"
#include <wam/bfs_organizer/bfs_organizer.h>
#include <string>
#include <wam/compiler/error/compiler_error.h>


using namespace wam;
using namespace std;


TEST_CASE("Div by 0 exception"){
    bfs_organizer org;
    org.load_query("X is 1 // 0.");
    auto res = org.get_answer();
    REQUIRE(res.is_error());
    REQUIRE(res.get_error().err_code == ERROR_TYPE::DIV_BY_0);
}

TEST_CASE("Arguments not suff instanciated"){
    bfs_organizer org;
    auto err = org.load_query("X is Y.");
    REQUIRE(err.exists());
    REQUIRE(err.type == compiler::ERROR_TYPE::ARGUMENTS_NOT_SUFF_INSTANCIATED);
}

TEST_CASE("Fun is not arithmetic exception"){
    bfs_organizer org;
    const char* prog =
            "c(a)."
            "f(g(a))."
            "l([]).";
    org.load_program(prog);
    org.load_query("f(Y), X is Y.");
    auto res = org.get_answer();
    REQUIRE(res.is_error());
    REQUIRE(res.get_error().err_code == ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC);

    org.load_query("l(Y), X is Y.");
    auto res_l = org.get_answer();
    REQUIRE(res_l.is_error());
    REQUIRE(res_l.get_error().err_code == ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC);

    org.load_query("c(Y), X is Y.");
    auto res_c = org.get_answer();
    REQUIRE(res_c.is_error());
    REQUIRE(res_c.get_error().err_code == ERROR_TYPE::FUNCTOR_IS_NOT_ARITHMETIC);
}

TEST_CASE("Div with / parser error"){
    bfs_organizer org;
    auto pe = org.validate_query("X is 1 / 0.");
    REQUIRE(pe.exists());
}

