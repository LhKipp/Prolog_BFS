//
// Created by leonhard on 08.05.20.
//

#include "test_util.h"
#include <numeric>
#include <catch2/catch.hpp>

std::string vec_to_string(const std::vector <std::string> &vec) {
    return std::accumulate(std::begin(vec), std::end(vec), std::string(),
                           [](std::string &ss, const std::string &s)
                           {
                               return ss.empty() ? s : ss + '\n' + s;
                           });
}

void has_all_of_these(std::optional<std::vector<wam::var_binding>> ans, std::initializer_list<std::string> substs) {
    REQUIRE(ans.has_value());
    REQUIRE(ans->size() == substs.size() / 2);

    std::map<std::string, std::string> actual_substs;
    for (auto i = substs.begin(); i != substs.end(); ++i) {
        auto var = *i;
        ++i;
        auto binding = *i;
        actual_substs[var] = binding;
    }

    for (auto &subst : *ans) {
        REQUIRE(actual_substs.at(subst.var_name) == subst.binding);
        actual_substs.erase(subst.var_name);
    }

    REQUIRE(actual_substs.empty());
}
