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
#include "../../data/functor_view.h"

struct node {
    STORED_OBJECT_FLAG type;
    std::string name;

    //functor exclusive field
    std::unique_ptr<std::vector<node>> children;

    //
    size_t x_reg = std::numeric_limits<size_t>::max();
    size_t a_reg = std::numeric_limits<size_t>::max();

    inline bool is_constant() const {
        return type == STORED_OBJECT_FLAG::CONSTANT;
    }

    inline bool is_variable() const {
        return type == STORED_OBJECT_FLAG::VARIABLE;
    }

    inline bool is_functor() const {
        return type == STORED_OBJECT_FLAG::FUNCTOR;
    }

    inline bool has_a_reg() const {
        return a_reg != std::numeric_limits<size_t>::max();
    }

    inline functor_view to_functor_view()const{
        if(is_constant()){
            return functor_view{name, 0};
        }else{
            return functor_view{name, (int)children->size()};
        }
    }


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

    node(const node &other) : type{other.type}, name{other.name}, a_reg{other.a_reg}, x_reg{other.x_reg} {
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
    }

    node &operator=(const node &other) {
        this->name = other.name;
        this->type = other.type;
        this->x_reg = other.x_reg;
        this->a_reg = other.a_reg;
//        this->first_declaration = other.first_declaration;
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
        return *this;
    }

    node &operator=(node &&other) = default;
};


#endif //PROLOG_BFS_NODE_H
