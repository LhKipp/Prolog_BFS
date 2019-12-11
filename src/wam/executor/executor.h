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
#include "../../util/inclusive_range.h"

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
        bfs_organizer *organizer = nullptr;

        //Pointer to the executor from whom this executor emerged in the call instruction
        const executor *parent = nullptr;

        //Every child reuses the heap of the parent. So the child heaps build upon the parents heap. Thous
        //the child-heap starts at index parent-heap.size and ends at parent-heap.size + child-heap.size
        size_t heap_start_index = 0;

        //Every child may need to overwrite some parts of the heap of the parent. As many Childs rely on one heap
        //a particular child is not allowed to write in parent heap. So we need to store local changes
        std::unordered_map<size_t, regist> changes_to_parent;

        std::vector<regist> heap;

    public:

        std::vector<var_reg_substitution> substitutions;
        int cur_atom_begin;

        executor& operator=(const executor & other)=default;
        executor& operator=(executor && other)=default;
        executor(executor&& parent) = default;
        executor(const executor& parent) = default;
        executor() = default;


        std::vector<regist> registers;

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
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        inline const functor_view &functor_of(STR_index STR_index) const {
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        functor_view &functor_of(FUN_index FUN_index);

        const functor_view &functor_of(FUN_index FUN_index) const;

        size_t index_of(const functor_view &functor)const;

        inline void push_back(const regist& regist){
            heap.emplace_back(regist);
        }
        inline void push_back_STR(){
            heap.emplace_back(heap_tag::STR, heap_start_index + heap_size() + 1);
        }
        inline void push_back_FUN(const functor_view & functor){
            heap.emplace_back(heap_tag::FUN, index_of(functor));
        }
        inline void push_back_unbound_REF() {
            heap.emplace_back(heap_tag::REF,heap_start_index + heap_size() );
        }
        inline size_t heap_size()const{
            return heap_start_index + heap.size();
        }

        inline regist heap_back()const {
            return heap.back();
        }

        inline regist& heap_modify(size_t index){
            //Assert that the index is within heap
            assert(index < heap_start_index + heap.size());
            //If within this executor
            if(heap_start_index <= index && index < heap_start_index + heap.size()){
                return heap.at(index - heap_start_index);
            }else{//index is within parents range, which this executor is not allowed to change
                //So we give back a regist stored as a local change to parent
                //First copy the parent regist
                changes_to_parent.insert({index, heap_at(index)});
                return changes_to_parent.at(index);
            }
        }
        inline regist heap_at(size_t index)const {
            //Assert that the index is within heap
            assert(index < heap_start_index + heap.size());
            //If within child
            if(heap_start_index <= index && index < heap_start_index + heap.size()){
                return heap.at(index - heap_start_index);
            }else{//index is within parents range
                //If this executor has overwritten the parent heap at index, return the change
                auto change = changes_to_parent.find(index);
                if(change != changes_to_parent.end()){
                    return change->second;
                }else{
                    //This executor has not overwritten the parent heap --> search in parent heap
                    return parent->heap_at(index);
                }
            }
        }

        inline void set_parent(const executor& parent){
            this->parent = &parent;
            heap_start_index = parent.heap_start_index + parent.heap.size();
        }




    };
}


#endif //PROLOG_BFS_EXECUTOR_H
