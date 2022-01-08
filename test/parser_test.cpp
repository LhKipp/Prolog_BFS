//
// Created by leonhard on 27.07.19.
//
//#include <iostream>
//#include <catch2/catch.hpp>
//#include "prolog_bfs/wam/compiler/compiler.h"
//#include "prolog_bfs/wam/compiler/util/data_enums.h"
//
//
//TEST_CASE("Parser tree_build", "[Parser]") {
//    SECTION("Tree 1") {
//        node root = wam::build_tree("p(Z,h(Z,W),f(W))");
//        wam::assign_registers(root.children->at(0));
//        REQUIRE(root.is_none());
//        node &p_func = root.children->at(0);
//        REQUIRE(p_func.is_functor());
//        REQUIRE(p_func.name == "p");
//        REQUIRE(p_func.children->size() == 3);
//
//        node &z_var = p_func.children->operator[](0);
//        REQUIRE(z_var.is_variable());
//        REQUIRE(z_var.name == "Z");
//        REQUIRE(z_var.get_a_reg() == 0);
//        REQUIRE(z_var.get_x_reg() == 3);
//
//        node &h_func = p_func.children->operator[](1);
//        REQUIRE(h_func.is_functor());
//        REQUIRE(h_func.name == "h");
//        REQUIRE(h_func.children->size() == 2);
//        REQUIRE(h_func.get_a_reg() == 1);
//
//        node &f_func = p_func.children->operator[](2);
//        REQUIRE(f_func.is_functor());
//        REQUIRE(f_func.name == "f");
//        REQUIRE(f_func.children->size() == 1);
//        REQUIRE(f_func.get_a_reg() == 2);
//
//        node &h_child_Z = h_func.children->operator[](0);
//        REQUIRE(h_child_Z.is_variable());
//        REQUIRE(h_child_Z.name == "Z");
//        REQUIRE(h_child_Z.get_x_reg() == 3);
//
//        node &h_child_W = h_func.children->operator[](1);
//        REQUIRE(h_child_W.is_variable());
//        REQUIRE(h_child_W.name == "W");
//        REQUIRE(h_child_W.get_x_reg() == 4);
//
//        node &f_child_W = f_func.children->operator[](0);
//        REQUIRE(f_child_W.is_variable());
//        REQUIRE(f_child_W.name == "W");
//        REQUIRE(f_child_W.get_x_reg() == 4);
//    }
//    SECTION("Tree 2") {
//        node root = wam::build_tree("p(f(X),h(Y,f(a)),Y)");
//        wam::assign_registers(root.children->at(0));
//        REQUIRE(root.is_none());
//
//        node &p_func = root.children->at(0);
//        REQUIRE(p_func.is_functor());
//        REQUIRE(p_func.children->size() == 3);
//        REQUIRE(p_func.name == "p");
//
//        node &f_func1 = p_func.children->operator[](0);
//        REQUIRE(f_func1.is_functor());
//        REQUIRE(f_func1.name == "f");
//        REQUIRE(f_func1.children->size() == 1);
//        REQUIRE(f_func1.get_a_reg() == 0);
//
//        node &x_var = f_func1.children->operator[](0);
//        REQUIRE(x_var.is_variable());
//        REQUIRE(x_var.name == "X");
//        REQUIRE(x_var.get_x_reg() == 4);
//
//        node &h_func = p_func.children->operator[](1);
//        REQUIRE(h_func.is_functor());
//        REQUIRE(h_func.name == "h");
//        REQUIRE(h_func.children->size() == 2);
//        REQUIRE(h_func.get_a_reg() == 1);
//
//        node &y_var = h_func.children->operator[](0);
//        REQUIRE(y_var.is_variable());
//        REQUIRE(y_var.name == "Y");
//        REQUIRE(y_var.get_x_reg() == 3);
//
//
//        node &f_func2 = h_func.children->operator[](1);
//        REQUIRE(f_func2.is_functor());
//        REQUIRE(f_func2.name == "f");
//        REQUIRE(f_func2.children->size() == 1);
//        REQUIRE(f_func2.get_x_reg() == 5);
//
//        node &a_cons = f_func2.children->operator[](0);
//        REQUIRE(a_cons.is_constant());
//        REQUIRE(a_cons.name == "a");
//        REQUIRE(a_cons.get_x_reg() == 6);
//
//        node &y_var2 = p_func.children->operator[](2);
//        REQUIRE(y_var2.is_variable());
//        REQUIRE(y_var2.name == "Y");
//        REQUIRE(y_var2.get_x_reg() == 3);
//        REQUIRE(y_var2.get_a_reg() == 2);
//    }
//    SECTION("Term with a list") {
//        node root = wam::build_tree("p(Z,[Z, W] ,f(W))");
//        REQUIRE(root.is_none());
//
//        node &p_func = root.children->at(0);
//        REQUIRE(p_func.is_functor());
//        REQUIRE(p_func.children->size() == 3);
//        REQUIRE(p_func.name == "p");
//
//        node &Z = p_func.children->at(0);
//        REQUIRE(Z.is_variable());
//        REQUIRE(Z.name == "Z");
//
//        node &l1 = p_func.children->at(1);
//        REQUIRE(l1.is_functor());
//        REQUIRE(l1.name == "[");
//        REQUIRE(l1.children->at(0).is_variable());
//        REQUIRE(l1.children->at(0).name == "Z");
//
//        node &l2 = l1.children->at(1);
//        REQUIRE(l2.is_functor());
//        REQUIRE(l2.name == "[");
//        REQUIRE(l2.children->at(0).is_variable());
//        REQUIRE(l2.children->at(0).name == "W");
//
//        node &l3 = l2.children->at(1);
//        REQUIRE(l3.is_functor());
//        REQUIRE(l3.name == "[");
//        REQUIRE(l3.is_empty_list());
//
//        node &f = p_func.children->at(2);
//        REQUIRE(f.is_functor());
//        REQUIRE(f.children->size() ==1);
//        REQUIRE(f.name == "f");
//
//        node & W = f.children->at(0);
//        REQUIRE(W.is_variable());
//        REQUIRE(W.name == "W");
//    }
//}

