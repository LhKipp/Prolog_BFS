//
// Created by leonhard on 09.03.20.
//

#ifndef PROLOG_BFS_SOURCE_CODE_INFO_H
#define PROLOG_BFS_SOURCE_CODE_INFO_H

#include <string>

namespace wam{

    struct source_code_info {
        unsigned line;
        //See util.cpp add_code_info for more info
//        unsigned column;
        std::string value;
    };

}
#endif //SOURCE_CODE_INFO_H
