//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_VAR_BINDING_H
#define PROLOG_BFS_VAR_BINDING_H

#include <string>
#include <ostream>
#include "../config/config.h"

namespace wam{
    struct var_binding{
        std::string var_name;
        std::string binding;

        var_binding() = default;
        var_binding(std::string varName, std::string binding) : var_name(std::move(varName)),
                                                                   binding(std::move(binding)) {}

        friend std::ostream &operator<<(std::ostream &os, const var_binding &substitution);

        bool operator==(const var_binding &rhs) const;

        bool operator!=(const var_binding &rhs) const;

        bool inline binds_to_var()const{
            return std::isupper(binding.at(0));
        }

        bool inline binds_to_default_var()const{
            return binding.find(config::UNBOUND_VAR_PREFIX) == 0;
        }

    };

    std::ostream &operator<<(std::ostream &os, const var_binding &substitution);


}

#endif //PROLOG_BFS_VAR_BINDING_H
