//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_HEAP_REG_H
#define PROLOG_BFS_HEAP_REG_H


#include "heap_tag.h"
#include <string>
#include <cassert>

namespace wam {
    struct heap_reg {
        int index;
        short var_index;
        heap_tag type;

        heap_reg()= default;
        heap_reg(heap_tag type, int index): heap_reg(type, index, 0){};
        heap_reg(heap_tag type, int index1, short index2): type{type}, index{index1}, var_index{index2}{};

        inline bool is_REF()const {
            return type == heap_tag::REF;
        }
        inline bool is_STR()const{
            return type == heap_tag ::STR;
        }
        inline bool is_FUN()const{
            return type == heap_tag ::FUN;
        }
        inline bool is_CONS()const{
            return type == heap_tag ::CONS;
        }
        inline bool is_INT()const{
            return type == heap_tag ::INT;
        }
        inline bool is_EVAL_FUN()const{
            return type == heap_tag ::EVAL_FUN;
        }

        inline int get_int_val()const{
            assert(is_INT());
            return index;
        }

        inline int get_cons_i()const{
            return index;
        }
        inline int get_eval_fun_i()const{
            return index;
        }
        inline int get_fun_i()const{
            return index;
        }
        inline int get_arity()const{
            assert(is_FUN() || is_EVAL_FUN());
            return var_index;
        }
        inline int get_heap_i()const{
            assert(is_REF() || is_STR());
            return index;
        }

        inline void bind_to(int index){
            assert(is_REF());
            this->index = index;
        }

        std::string type_as_str();

        /*
         * This is shallow. For functors there is no check if all children regs are equal
         */
        bool operator==(const heap_reg &rhs) const;

        bool operator!=(const heap_reg &rhs) const;
    };

}
#endif //PROLOG_BFS_HEAP_REG_H
