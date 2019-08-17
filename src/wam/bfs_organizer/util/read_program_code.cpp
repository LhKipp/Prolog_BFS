//
// Created by leonhard on 17.08.19.
//

#include "read_program_code.h"
#include <algorithm>
#include <fstream>


std::vector<std::string> wam::read_program_code(const std::vector<std::string> &program_lines) {
    std::vector<std::string> result{};
    result.reserve(program_lines.size());

    std::string cur_term;

    for(const auto& prog_line : program_lines){
        auto first_char = std::find_if_not(prog_line.begin(), prog_line.end(), [](auto& ch){return std::isspace(ch);});
        if(*first_char == '%'){
            continue;
        }
        cur_term += prog_line;

        auto last_char = std::find_if_not(prog_line.rbegin(), prog_line.rend(), [](auto& ch){return std::isspace(ch);});
        if(*last_char == '.'){
            result.push_back(cur_term);
            cur_term ="";
        }
    }
    return result;
}

std::vector<std::string> wam::read_program_code(const std::string& file_path){
    std::vector<std::string> program_lines;

    std::ifstream file{file_path};
    std::string cur;
    while(std::getline(file,cur)){
        program_lines.push_back(cur);
    }

    return read_program_code(program_lines);
}
