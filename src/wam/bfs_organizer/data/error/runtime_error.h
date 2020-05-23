//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_RUNTIME_ERROR_H
#define PROLOG_BFS_RUNTIME_ERROR_H

#include <wam/data/source_code_info.h>
#include "runtime_error_type.h"

namespace wam{


    struct runtime_error{
        ERROR_TYPE err_code;
        source_code_info atom_with_err;
        std::string explanation;

        runtime_error(): err_code{ERROR_TYPE::NONE} {}

        runtime_error(ERROR_TYPE err_type, std::string explanation): err_code{err_type},
            explanation{std::move(explanation)}{}
        runtime_error(ERROR_TYPE errCode, const source_code_info &atomWithErr, std::string_view explanation)
                : err_code(errCode), atom_with_err(atomWithErr), explanation(explanation) {}

        bool is_present()const{
            return err_code != ERROR_TYPE ::NONE;
        }

        std::string get_type_as_str()const{
            return ((const char *[]) {
                    "DIV BY 0",
            }[static_cast<size_t>(err_code)]);
        }
        int get_err_line()const{
            return atom_with_err.line_begin;
        }
        const std::string& get_err_atom_as_str()const{
            return atom_with_err.value;
        }

        const std::string& get_explanation()const{
            return explanation;
        }

    };
}

#endif //PROLOG_BFS_RUNTIME_ERROR_H
