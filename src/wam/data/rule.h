//
// Created by leonhard on 14.03.20.
//

#ifndef PROLOG_BFS_RULE_H
#define PROLOG_BFS_RULE_H

#include <vector>
#include "compiled_atom.h"

namespace wam{
    struct rule{
    private:
        std::vector<compiled_atom> _atoms;

    public:
        rule(){}
        rule(const std::vector<compiled_atom> &atoms) : _atoms(atoms) {}
        rule(std::vector<compiled_atom> &&atoms) : _atoms(std::move(atoms)) {}

        std::vector<compiled_atom>& atoms(){
            return _atoms;
        };

        const std::vector<compiled_atom>& atoms()const{
            return _atoms;
        };
    };
}
#endif //PROLOG_BFS_RULE_H
