//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_COMPILER_ERROR_H
#define PROLOG_BFS_COMPILER_ERROR_H

#include <wam/data/source_code_info.h>
#include "compiler_error_type.h"

namespace compiler{
    struct error{
        ERROR_TYPE type;
        wam::source_code_info source_info;
        std::string description;
        unsigned column;

        error():type{ERROR_TYPE::NONE}{}
        error(ERROR_TYPE type,
              const wam::source_code_info &source_info,
              const std::string &description,
              unsigned column = std::numeric_limits<int>::max()) : type(type), source_info(source_info), description(description), column{column} {
        }

        std::string get_description()const{
            return description;
        }

        bool has_column_info()const{
            return column != std::numeric_limits<int>::max();
        }

        bool exists()const{
            return type != ERROR_TYPE ::NONE;
        }

        unsigned get_row()const{
            return source_info.line_begin;
        }

        unsigned get_col()const{
            return column;
        }
    };
}

#endif //PROLOG_BFS_COMPILER_ERROR_H
