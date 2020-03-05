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
#include "../data/var_heap_substitution.h"

#include <vector>
#include <stack>
#include <unordered_map>
#include <cassert>
#include <iostream>

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
        bfs_organizer *organizer = nullptr;

        //Pointer to the executor from whom this executor emerged in the call instruction
        //Note: size_t::max represents that this exec has no parent -- see @func has_parent
        size_t parent_index = std::numeric_limits<size_t>::max();

        //Every child reuses the heap of the parent. So the child heaps build upon the parents heap. Thous
        //the child-heap starts at index parent-heap.size and ends at parent-heap.size + child-heap.size
        size_t heap_start_index = 0;

        //Every child may need to overwrite some parts of the heap of the parent. As many Childs rely on one heap
        //a particular child is not allowed to write in parent heap. So we need to store local changes
        std::unordered_map<size_t, regist> changes_to_parent{};

        std::vector<regist> heap{};

    public:
        //Set in instruction point_var_regs_to_heap
        //The var_reg_substs are pointing into the heap
        std::vector<var_heap_substitution> substitutions;
        //We also need to keep track whether the var_heap_substitutions
        //are from an original user entered query
        //the information is stored in solves_term_code.is_from_original_query()
        wam::term_code* solves_term_code;
        inline bool is_from_user_entered_query() const{
            return solves_term_code->is_from_original_query();
        }

        //TODO is this field necessary?
        std::vector<var_substitution> found_substitutions;

        //TODO is this field necessary
        int cur_atom_begin;

        std::vector<regist> registers;

        mode read_or_write;

        size_t S;

        bool fail = false;
        //Which atom number from the original query the executor is solving depends on the call instruction.
        //The executor is solving the first (zeroth) atom from the query if the query-atom has been built on the heap,
        //and the call instruction suceeded. Thous this data member should only be written by the call instruction
        int solves_atom_number = -1;

        std::stack<term_code*> instructions;
        std::stack<wam::environment> environments;

        executor& operator=(const executor & other)=default;
        executor& operator=(executor && other)=default;
        executor(const executor& other) = default;
        executor() = default;

        inline std::vector<wam::regist>& cur_permanent_registers(){
            assert(!environments.empty());
            return environments.top().permanent_registers;
        }


        inline bfs_organizer *get_organizer() const {
            return organizer;
        }

        inline functor_view &functor_of(STR_index STR_index) {
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        inline const functor_view &functor_of(STR_index STR_index) const {
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        functor_view &functor_of(FUN_index FUN_index);

        const functor_view &functor_of(FUN_index FUN_index) const;

        size_t index_of(const functor_view &functor)const;

        inline void push_back(const regist& regist){
            heap.push_back(regist);
        }
        inline void push_back_STR(){
            heap.emplace_back(heap_tag::STR,  heap_size() + 1);
        }
        inline void push_back_FUN(const functor_view & functor){
            heap.emplace_back(heap_tag::FUN, index_of(functor));
        }
        inline void push_back_unbound_REF() {
            heap.emplace_back(heap_tag::REF,heap_size());
        }
        inline size_t heap_size()const{
            return heap_start_index + heap.size();
        }

        regist heap_back()const;

        regist& heap_modify(size_t index);

        regist heap_at(size_t index)const;

        inline void set_parent(const executor& parent, size_t archive_index){
            parent_index = archive_index;
            heap_start_index = parent.heap_size();

            //Necessary copies - for now
            organizer = parent.organizer;
            environments = parent.environments;//TODO use parent environments
            //TODO figure out whether the registers are only necessary to copy from
            //head func -> first body atom
            registers = parent.registers;
            instructions = parent.instructions;

            //TODO Monitor different heuristics
            changes_to_parent.reserve(5);
            heap.reserve(parent.heap.size());
        }

        inline bool has_parent()const {
            return parent_index != std::numeric_limits<size_t>::max();
        }
    };
}


#endif //PROLOG_BFS_EXECUTOR_H
