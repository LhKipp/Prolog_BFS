//
// Created by leonhard on 17.08.19.
//

#ifndef PROLOG_BFS_READ_PROGRAM_CODE_H
#define PROLOG_BFS_READ_PROGRAM_CODE_H

#include <vector>
#include <string>

namespace wam {
    std::vector <std::string> read_program_code(const std::vector <std::string> &program_lines);


    std::vector <std::string> read_program_code(const std::string &file_path);
}
#endif //PROLOG_BFS_READ_PROGRAM_CODE_H
