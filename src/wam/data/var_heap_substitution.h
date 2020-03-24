//
// Created by leonhard on 05.03.20.
//

#ifndef PROLOG_BFS_VAR_HEAP_SUBSTITUTION_H
#define PROLOG_BFS_VAR_HEAP_SUBSTITUTION_H
#include <string>

namespace wam {
    struct var_heap_substitution {

        std::string var_name;
        size_t heap_index;

        //TODO fix the substitution bug like this

        var_heap_substitution() = default;

        var_heap_substitution(std::string varName, size_t heap_index) :
                var_name(std::move(varName)),
                heap_index(heap_index){
        }

        bool operator==(const var_heap_substitution &rhs) const {
            return var_name == rhs.var_name && heap_index == rhs.heap_index;
        }

        bool operator!=(const var_heap_substitution &rhs) const {
            return !(rhs == *this);
        }

    };
}
#endif //PROLOG_BFS_VAR_HEAP_SUBSTITUTION_H
