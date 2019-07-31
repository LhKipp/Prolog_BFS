//
// Created by leonhard on 24.07.19.
//

#ifndef PROLOG_BFS_EXECUTOR_H
#define PROLOG_BFS_EXECUTOR_H


#include "../data/regist.h"
#include "../data/functor_view.h"
#include "util/mode.h"
#include "../data/var_reg_substitution.h"
#include "../data/term_code.h"
#include "../../util/named_type.h"
#include <vector>
#include <unordered_map>

namespace wam {

    struct STR_Parameter{};
    struct FUN_Parameter{};
    using FUN_index = NamedType<size_t ,FUN_Parameter>;
    using STR_index = NamedType<size_t ,STR_Parameter>;

    struct executor {

        friend struct bfs_organizer;

    public:
        executor()=default;

        std::vector<regist> registers;
        std::vector<regist> heap;

        mode read_or_write;

        size_t S;

        bool fail = false;

        //Pointer to global functor storage
        std::vector<functor_view>* functors;

        inline functor_view& functor_of(STR_index STR_index){
            return functor_of(FUN_index{heap[STR_index.get()].index});
        }
        inline const functor_view& functor_of(STR_index STR_index)const {
            return functor_of(FUN_index{heap[STR_index.get()].index});
        }

        inline functor_view& functor_of(FUN_index FUN_index){
            return functors->operator[](heap[FUN_index.get()].index);
        }
        inline const functor_view& functor_of(FUN_index FUN_index)const {
            return functors->operator[](heap[FUN_index.get()].index);
        }

        inline size_t index_of(const functor_view & functor){
            auto search = functor_view_regs->find(functor);
            //If we have seen this functor already
            if (search != functor_view_regs->end()) {
                return search->second;
            } else {
                auto index = functors->size();
                functors->push_back(functor);
                functor_view_regs->operator[](functor)= index;
                return index;
            }
        }
    private:
        //Pointer to global functor_to_index map
        std::unordered_map<functor_view, size_t>* functor_view_regs;

        wam::term_code* cur_prog_code;
    };
}


#endif //PROLOG_BFS_EXECUTOR_H
