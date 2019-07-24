#ifndef PROLOG_BFS_CONSTANT_H
#define PROLOG_BFS_CONSTANT_H

#include <string>

namespace prolog {
    struct constant {
        std::string name;

        constant(){
            name = "";
        };
        constant(const std::string name) {
            this->name = name;
        }

        bool operator==(const prolog::constant &other) {
            return this->name == other.name;
        }
    };
}

#endif PROLOG_BFS_CONSTANT_H