//
// Created by leonhard on 27.07.19.
//
#include "../catch.hpp"
#include "../src/wam/parser/parser.h"
#include "../src/prolog/data/data_types/data_enums.h"


TEST_CASE("Parser tree_build", "[Parser]"){

    node root = wam::build_tree("p(Z,h(Z,W),f(W))");
    REQUIRE(root.type == NONE);
    node& p_func = root.children->operator[](0);
    REQUIRE(p_func.type== FUNCTOR);
    REQUIRE(p_func.name=="p");
    REQUIRE(p_func.children->size() == 3);

    node& z_var = p_func.children->operator[](0);
    REQUIRE(z_var.type== VARIABLE);
    REQUIRE(z_var.name=="Z");

    node& h_func = p_func.children->operator[](1);
    REQUIRE(h_func.type== FUNCTOR);
    REQUIRE(h_func.name=="h");
    REQUIRE(h_func.children->size() == 2);

    node& f_func = p_func.children->operator[](2);
    REQUIRE(f_func.type== FUNCTOR);
    REQUIRE(f_func.name=="f");
    REQUIRE(f_func.children->size() == 1);

    node& h_child_Z = h_func.children->operator[](0);
    REQUIRE(h_child_Z.type== VARIABLE);
    REQUIRE(h_child_Z.name=="Z");

    node& h_child_W = h_func.children->operator[](1);
    REQUIRE(h_child_W.type== VARIABLE);
    REQUIRE(h_child_W.name=="W");

    node& f_child_W = f_func.children->operator[](0);
    REQUIRE(f_child_W.type== VARIABLE);
    REQUIRE(f_child_W.name=="W");
}

TEST_CASE("Parser register assignments", "[Parser]"){
    wam::executor exe;
    node top_node = wam::build_tree("p(Z,h(Z,W),f(W))");
    wam::assign_registers(exe, top_node);

    std::vector<wam::regist>& regs = exe.registers;
    REQUIRE(regs.size()== 5);
    REQUIRE(regs[0].type == wam::FUN);
    REQUIRE(exe.functors[regs[0].index].name == "p");
    REQUIRE(exe.functors[regs[0].index].arity == 3);

    REQUIRE(regs[1].type == wam::REF);
    REQUIRE(regs[1].index == 1);

    REQUIRE(regs[2].type == wam::FUN);
    REQUIRE(exe.functors[regs[2].index].name == "h");
    REQUIRE(exe.functors[regs[2].index].arity == 2);

    REQUIRE(regs[3].type == wam::FUN);
    REQUIRE(exe.functors[regs[3].index].name == "f");
    REQUIRE(exe.functors[regs[3].index].arity == 1);

    REQUIRE(regs[4].type == wam::REF);
    REQUIRE(regs[4].index == 4);
}