//
// Created by leonhard on 17.08.19.
//

#include "read_program_code.h"
#include <algorithm>
#include <fstream>
#include <iostream>


std::vector<std::string> wam::read_program_code(const std::vector<std::string> &program_lines) {
    std::vector<std::string> result{};
    result.reserve(program_lines.size());

    std::string cur_term;

    for (auto &prog_line : program_lines) {
        auto percent_sign = std::find(prog_line.begin(), prog_line.end(), '%');

        std::copy(prog_line.begin(), percent_sign, std::back_inserter(cur_term));

        auto last_char = std::find_if_not(prog_line.rbegin(), prog_line.rend(),
                                          [](auto &ch) { return std::isspace(ch); });
        if (*last_char == '.') {
            if (std::count_if(cur_term.begin(), cur_term.end(), [](auto ch) { return !std::isspace(ch); }) == 0) {
                cur_term = "";
                continue;
            }
            result.push_back(cur_term);
            cur_term = "";
        }
    }
    return result;
}

std::vector<std::string> wam::read_program_code(const std::string &file_path) {
    std::vector<std::string> program_lines;

    std::ifstream file{file_path};
    if(!file){
        std::cout << "Couldn't read program code from file: "<< file_path << std::endl;
        return program_lines;
    }
    std::string cur;
    while (std::getline(file, cur)) {
        program_lines.push_back(cur);
    }

    return read_program_code(program_lines);
}
