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
#include "../data/var_substitution.h"
#include "../data/environment.h"

#include <vector>
#include <stack>
#include <unordered_map>
#include <cassert>


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

        std::vector<var_reg_substitution> substitutions;
        int cur_atom_begin;

        executor() = default;
        executor(const executor& other) = default;
        executor& operator=(const executor & other)=default;
        executor& operator=(executor && other)=default;

        std::vector<regist> registers;
        std::vector<regist> heap;

        mode read_or_write;

        size_t S;

        bool fail = false;
        //Which atom number from the original query the executor is solving depends on the call instruction.
        //The executor is solving the first (zeroth) atom from the query if the query-atom has been built on the heap,
        //and the call instruction suceeded. Thous this data member should only be written by the call instruction
        int solves_atom_number = -1;

        std::stack<const term_code*> instructions;
        std::stack<wam::environment> environments;

        inline std::vector<wam::regist>& cur_permanent_registers(){
            assert(!environments.empty());
            return environments.top().permanent_registers;
        }

        std::vector<var_substitution> found_substitutions;

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

        size_t index_of(const functor_view &functor)const;

        inline void push_back_FUN(const functor_view & functor){
            heap.emplace_back(heap_tag::FUN, index_of(functor));
        }
        inline void push_back_unbound_REF() {
            heap.emplace_back(heap_tag::REF, heap.size() );
        }


    };
}


#endif //PROLOG_BFS_EXECUTOR_H
