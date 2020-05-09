//
// Created by leonhard on 09.05.20.
//

#ifndef PROLOG_BFS_RESULT_H
#define PROLOG_BFS_RESULT_H

#include <variant>
#include <optional>
#include <vector>
#include <wam/data/var_binding.h>
#include <wam/bfs_organizer/data/error/runtime_error.h>

namespace wam{
    struct result{

        std::variant<std::optional<std::vector<wam::var_binding>>, wam::runtime_error> data;

        bool is_error()const{
            return std::holds_alternative<wam::runtime_error>(data);
        }

        bool is_answer()const{
            return std::holds_alternative<std::optional<std::vector<wam::var_binding>>>(data);
        }

        const wam::runtime_error& get_error()const{
            return std::get<1>(data);
        }

        const std::optional<std::vector<wam::var_binding>>& get_answer()const{
            return std::get<0>(data);
        }

        result(const std::optional<std::vector<wam::var_binding>> &ans) : data(ans) {}
        result(const wam::runtime_error& err) : data(err) {}
    };
}

#endif //PROLOG_BFS_RESULT_H
