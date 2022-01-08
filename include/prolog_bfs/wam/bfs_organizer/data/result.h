//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_RESULT_H
#define PROLOG_BFS_RESULT_H

#include <variant>
#include <optional>
#include <vector>
#include <prolog_bfs/wam/data/var_binding.h>
#include <prolog_bfs/wam/bfs_organizer/data/error/runtime_error.h>
#include <cassert>
#include "runtime_statistics.h"

namespace wam{
    struct result{

        std::variant<std::optional<std::vector<wam::var_binding>>, wam::runtime_error> data;
        wam::runtime_statistics statistics;

        const wam::runtime_statistics& get_statistics()const{
            return statistics;
        }

        bool is_error()const{
            return std::holds_alternative<wam::runtime_error>(data);
        }

        bool is_answer()const{
            return std::holds_alternative<std::optional<std::vector<wam::var_binding>>>(data);
        }

        const wam::runtime_error& get_error()const{
            assert(is_error());
            return std::get<1>(data);
        }

        const std::optional<std::vector<wam::var_binding>>& get_answer()const{
            assert(is_answer());
            return std::get<0>(data);
        }
        
        std::string get_answer_as_str() const;

        result(const std::optional<std::vector<wam::var_binding>> &ans, const wam::runtime_statistics& stats) : data(ans), statistics{stats} {}
        result(const wam::runtime_error& err, const wam::runtime_statistics& stats) : data(err), statistics{stats}{}
    };
}

#endif //PROLOG_BFS_RESULT_H
