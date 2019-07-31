//
// Created by leonhard on 27.07.19.
//
#include <iostream>
#include "../catch.hpp"
#include "../src/wam/parser/parser.h"
#include "../src/prolog/data/data_types/data_enums.h"


TEST_CASE("Parser tree_build", "[Parser]"){
    SECTION("Tree 1"){
        node root = wam::build_tree("p(Z,h(Z,W),f(W))");
        REQUIRE(root.type == STORED_OBJECT_FLAG ::NONE);
        node& p_func = root.children->operator[](0);
        REQUIRE(p_func.type== STORED_OBJECT_FLAG ::FUNCTOR);
        REQUIRE(p_func.name=="p");
        REQUIRE(p_func.children->size() == 3);

        node& z_var = p_func.children->operator[](0);
        REQUIRE(z_var.type== STORED_OBJECT_FLAG ::VARIABLE);
        REQUIRE(z_var.name=="Z");

        node& h_func = p_func.children->operator[](1);
        REQUIRE(h_func.type== STORED_OBJECT_FLAG ::FUNCTOR);
        REQUIRE(h_func.name=="h");
        REQUIRE(h_func.children->size() == 2);

        node& f_func = p_func.children->operator[](2);
        REQUIRE(f_func.type== STORED_OBJECT_FLAG ::FUNCTOR);
        REQUIRE(f_func.name=="f");
        REQUIRE(f_func.children->size() == 1);

        node& h_child_Z = h_func.children->operator[](0);
        REQUIRE(h_child_Z.type== STORED_OBJECT_FLAG ::VARIABLE);
        REQUIRE(h_child_Z.name=="Z");

        node& h_child_W = h_func.children->operator[](1);
        REQUIRE(h_child_W.type== STORED_OBJECT_FLAG ::VARIABLE);
        REQUIRE(h_child_W.name=="W");

        node& f_child_W = f_func.children->operator[](0);
        REQUIRE(f_child_W.type== STORED_OBJECT_FLAG ::VARIABLE);
        REQUIRE(f_child_W.name=="W");
    }
    SECTION("Tree 2"){
        node root = wam::build_tree("p(f(X),h(Y,f(a)),Y)");
        wam::executor exe;
        wam::assign_registers( root);
        REQUIRE(root.type == STORED_OBJECT_FLAG::NONE);

        node &p_func = root.children->at(0);
        REQUIRE(p_func.type == STORED_OBJECT_FLAG::FUNCTOR);
        REQUIRE(p_func.children->size() == 3);
        REQUIRE(p_func.name == "p");
        REQUIRE(p_func.regist ==0);

        node &f_func1 = p_func.children->operator[](0);
        REQUIRE(f_func1.type == STORED_OBJECT_FLAG::FUNCTOR);
        REQUIRE(f_func1.name == "f");
        REQUIRE(f_func1.children->size() == 1);
        REQUIRE(f_func1.regist ==1);

        node &x_var = f_func1.children->operator[](0);
        REQUIRE(x_var.type == STORED_OBJECT_FLAG::VARIABLE);
        REQUIRE(x_var.name == "X");
        REQUIRE(x_var.regist == 4);

        node &h_func = p_func.children->operator[](1);
        REQUIRE(h_func.type == STORED_OBJECT_FLAG::FUNCTOR);
        REQUIRE(h_func.name == "h");
        REQUIRE(h_func.children->size() == 2);
        REQUIRE(h_func.regist == 2);

        node &y_var = h_func.children->operator[](0);
        REQUIRE(y_var.type == STORED_OBJECT_FLAG::VARIABLE);
        REQUIRE(y_var.name == "Y");
        REQUIRE(y_var.regist == 3);


        node &f_func2 = h_func.children->operator[](1);
        REQUIRE(f_func2.type == STORED_OBJECT_FLAG::FUNCTOR);
        REQUIRE(f_func2.name == "f");
        REQUIRE(f_func2.children->size() == 1);
        REQUIRE(f_func2.regist == 5);

        node &a_cons = f_func2.children->operator[](0);
        REQUIRE(a_cons.type == STORED_OBJECT_FLAG::CONSTANT);
        REQUIRE(a_cons.name == "a");
        REQUIRE(a_cons.regist == 6);

        node &y_var2 = p_func.children->operator[](2);
        REQUIRE(y_var2.type == STORED_OBJECT_FLAG::VARIABLE);
        REQUIRE(y_var2.name == "Y");
        REQUIRE(y_var2.regist == 3);
    }
}

