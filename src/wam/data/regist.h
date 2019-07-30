//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_REGIST_H
#define PROLOG_BFS_REGIST_H


#include "heap_tag.h"
#include <cstddef>
#include <vector>

namespace wam {
    struct regist {
        heap_tag type;
        size_t index;

        regist()= default;
        regist(heap_tag type, size_t index): type{type}, index{index}{}

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

inline bool is_REF(const std::vector<wam::regist>& store, size_t addr){
    return store[addr].is_REF();
}


#endif //PROLOG_BFS_REGIST_H
