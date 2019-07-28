//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_NODE_H
#define PROLOG_BFS_NODE_H


#include <string>
#include <vector>
#include <memory>
#include <variant>
#include "../../../prolog/data/data_types/data_enums.h"
#include "../../data/regist.h"

struct node {
    STORED_OBJECT_FLAG type;
    std::string name;

    //functor exclusive field
    std::unique_ptr<std::vector<node>> children;

//    //Variable exclusive field (If a variable has been declared before, first declaration will point to the register
//    node* first_declaration;

    //
    size_t regist;

    explicit node(const STORED_OBJECT_FLAG type) : node(type, "") {};

    node(const STORED_OBJECT_FLAG type, const std::string name) : name(name), type(type) {
        if (type == STORED_OBJECT_FLAG::FUNCTOR) {
            children = std::make_unique<std::vector<node>>();
        } else {
            children = nullptr;
        }
    }

    node() : type{STORED_OBJECT_FLAG::NONE}, name{}, children{} {}

    node(node &&other) = default;

    node(const node &other) : type{other.type}, name{other.name} {
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
    }

    node &operator=(const node &other) {
        this->name = other.name;
        this->type = other.type;
//        this->first_declaration = other.first_declaration;
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
        return *this;
    }

    node &operator=(node &&other) = default;
};


#endif //PROLOG_BFS_NODE_H
