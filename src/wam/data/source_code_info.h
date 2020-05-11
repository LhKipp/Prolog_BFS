//
// Created by leonhard on 09.03.20.
//

#ifndef PROLOG_BFS_SOURCE_CODE_INFO_H
#define PROLOG_BFS_SOURCE_CODE_INFO_H

#include <string>
namespace wam{

    struct source_code_info {
        unsigned line_begin;
        unsigned line_end;
        std::string value;
        //See util.cpp add_code_info for more info
//        unsigned column;

        source_code_info() {}
        source_code_info(unsigned int lineBegin, unsigned int lineEnd, const std::string &value) : line_begin(
                lineBegin), line_end(lineEnd), value(value) {}
    };

    inline source_code_info code_info_for_built_in(std::string built_in_pred){
        return source_code_info{std::numeric_limits<unsigned>::max(),
                                std::numeric_limits<unsigned>::max(),
                                built_in_pred};
    }
}
#endif //SOURCE_CODE_INFO_H
