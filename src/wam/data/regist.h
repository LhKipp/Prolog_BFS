//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_REGIST_H
#define PROLOG_BFS_REGIST_H


#include "heap_tag.h"
#include <cstddef>

namespace wam {
    struct regist {
        int index;
        short var_index;
        heap_tag type;

        regist()= default;
        regist(heap_tag type, int index): type{type}, index{index}{}
        regist(int index, short var_index): type{heap_tag::REF}, index{index}, var_index{var_index}{}

        inline bool is_REF()const {
            return type == heap_tag::REF;
        }
        inline bool is_STR()const{
            return type == heap_tag ::STR;
        }
        inline bool is_FUN()const{
            return type == heap_tag ::FUN;
        }
    };
}



#endif //PROLOG_BFS_REGIST_H
