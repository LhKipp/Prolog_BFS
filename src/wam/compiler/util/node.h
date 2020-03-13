//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_NODE_H
#define PROLOG_BFS_NODE_H


#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <cassert>
#include <optional>
#include <ostream>
#include "data_enums.h"
#include "../../data/regist.h"
#include "../../data/source_code_info.h"
#include "../../data/functor_view.h"

struct node {
private:
    size_t x_reg = std::numeric_limits<size_t>::max();
    size_t a_reg = std::numeric_limits<size_t>::max();
    size_t y_reg = std::numeric_limits<size_t>::max();
    STORED_OBJECT_FLAG type = STORED_OBJECT_FLAG ::NONE;

public:

    wam::source_code_info code_info;

    std::string name;

    //FUNCTOR / LIST exclusive field
    std::unique_ptr<std::vector<node>> children;

    void add_to_children(const node& child){
        children->push_back(child);
    }

    //LIST exlusiv field
    bool is_list_begin = false;
    bool is_finished_list = false;

    inline void set_type(STORED_OBJECT_FLAG type){
        this->type = type;
    }
    inline bool is_argument() const {
        return a_reg != std::numeric_limits<size_t>::max();
    }
    inline size_t get_a_reg() const{
        assert(is_argument());
        return a_reg;
    }
    inline void set_a_reg(size_t index){
        a_reg= index;
    }

    inline size_t get_x_reg() const {
        assert(x_reg != std::numeric_limits<size_t>::max());
        return x_reg;
    }

    inline void set_x_reg(size_t index) {
        x_reg = index;
    }

    inline bool is_permanent() const{
        return y_reg != std::numeric_limits<size_t>::max();
    }
    inline size_t get_y_reg() const {
        assert(is_permanent() && y_reg != std::numeric_limits<size_t>::max());
        return y_reg;
    }

    inline void set_y_reg(size_t index) {
        y_reg = index;
    }

    inline bool is_constant() const {
        return type == STORED_OBJECT_FLAG::CONSTANT;
    }

    inline bool is_variable() const {
        return type == STORED_OBJECT_FLAG::VARIABLE;
    }

    inline bool is_functor() const {
        return type == STORED_OBJECT_FLAG::FUNCTOR;
    }
    inline bool is_list() const {
        return name == "[";
    }
    inline bool is_empty_list() const {
        return is_list() && children->empty();
    }
    inline bool is_non_empty_list()const{
        return is_list() && !children->empty();
    }
    inline bool is_non_full_list()const{
       return is_list() && children->size() < 2;
    }
    inline bool has_tail()const{
        assert(is_list());
        return children->size() == 2;
    }
    inline bool is_append_functor()const{
        return name == "|";
    }
    inline bool is_none()const{
        return type == STORED_OBJECT_FLAG ::NONE;
    }

    inline wam::functor_view to_functor_view() const {
        if (is_constant()) {
            return wam::functor_view{name, 0};
        } else {
            return wam::functor_view{name, (int) children->size()};
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

    node(const node &other) : type{other.type}, name{other.name}, a_reg{other.a_reg}, x_reg{other.x_reg},
                              code_info{other.code_info}{
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
    }

    node &operator=(const node &other) {
        this->name = other.name;
        this->type = other.type;
        this->x_reg = other.x_reg;
        this->a_reg = other.a_reg;
        if (other.children) {
            children = std::make_unique<std::vector<node>>(*other.children);
        }
        this->code_info = other.code_info;
        return *this;
    }

    node &operator=(node &&other) = default;

    friend std::ostream &operator<<(std::ostream &os, const node &node) {
        os << "node: name: " << node.name;
        return os;
    }

};


#endif //PROLOG_BFS_NODE_H
