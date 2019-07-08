#ifndef PROLOG_BFS_ATOM_H
#define PROLOG_BFS_ATOM_H

#include <string>

namespace prolog {
    struct atom {
        std::string name;

        atom(const std::string name) {
            this->name = name;
        }

        bool operator==(const prolog::atom &other) {
            return this->name == other.name;
        }
    };
}


namespace std {
    template<>
    struct hash<prolog::atom> {
        size_t operator()(const prolog::atom &atom) const {
            return hash<std::string>()(atom.name);
        }
    };
}

#endif PROLOG_BFS_ATOM_H