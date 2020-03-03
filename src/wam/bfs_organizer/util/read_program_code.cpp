//
// Created by leonhard on 17.08.19.
//

#include "read_program_code.h"
#include <fstream>
#include <streambuf>


std::string wam::read_file(const std::string_view file_path) {
    //https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream t;
    t.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    t.open(file_path.data());

    std::string str;
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());
    return str;
}

