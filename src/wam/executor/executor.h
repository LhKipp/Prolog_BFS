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

    class bfs_organizer;

    //Strong wrapper types
    struct STR_Parameter {
    };
    struct FUN_Parameter {
    };
    using FUN_index = NamedType<size_t, FUN_Parameter>;
    using STR_index = NamedType<size_t, STR_Parameter>;

    struct executor {

        friend class bfs_organizer;

    private:

        //Pointer to the organizer instance this executor is handled by
        //The organizer holds data like, Functors, Code, etc.
        bfs_organizer *organizer;
    public:
        executor() = default;

        std::vector<regist> registers;
        std::vector<regist> heap;

        mode read_or_write;

        size_t S;

        bool fail = false;
        bool finished = false;

        wam::term_code *cur_prog_code;//P
        wam::term_code * continuation_point; //CP
        std::forward_list<term_code*> instructions;

        inline bfs_organizer *get_organizer() const {
            return organizer;
        }

        inline functor_view &functor_of(STR_index STR_index) {
            return functor_of(FUN_index{heap[STR_index.get()].index});
        }

        inline const functor_view &functor_of(STR_index STR_index) const {
            return functor_of(FUN_index{heap[STR_index.get()].index});
        }

        functor_view &functor_of(FUN_index FUN_index);

        const functor_view &functor_of(FUN_index FUN_index) const;

        size_t index_of(const functor_view &functor);

        inline void push_back_FUN(const functor_view & functor){
            heap.emplace_back(heap_tag::FUN, index_of(functor));
        }
        inline void push_back_unbound_REF() {
            heap.emplace_back(heap_tag::REF, heap.size() );
        }


    };
}


#endif //PROLOG_BFS_EXECUTOR_H
