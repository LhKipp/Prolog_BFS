//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_VAR_SUBSTITUTION_H
#define PROLOG_BFS_VAR_SUBSTITUTION_H

#include <string>
#include <ostream>

namespace wam{
    struct var_substitution{
        std::string var_name;
        std::string substitute;

        var_substitution() = default;
        var_substitution(std::string varName, std::string substitute) : var_name(std::move(varName)),
                                                                                      substitute(std::move(substitute)) {}

        friend std::ostream &operator<<(std::ostream &os, const var_substitution &substitution);
    };

    std::ostream &operator<<(std::ostream &os, const var_substitution &substitution);


}

#endif //PROLOG_BFS_VAR_SUBSTITUTION_H
