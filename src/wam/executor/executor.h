//
// Created by leonhard on 24.07.19.
//

#ifndef PROLOG_BFS_EXECUTOR_H
#define PROLOG_BFS_EXECUTOR_H


#include "wam/data/heap_reg.h"
#include "../data/functor_view.h"
#include "util/mode.h"
#include "../data/var_reg_substitution.h"
#include "../data/compiled_atom.h"
#include "../../util/named_type.h"
#include "../data/var_binding.h"
#include "../data/environment.h"
#include "../data/var_heap_substitution.h"
#include "util/exec_state.h"

#include <vector>
#include <stack>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <variant>
#include <wam/bfs_organizer/data/error/runtime_error.h>
#include <wam/data/heap_reg_with_i.h>

namespace wam {

    class bfs_organizer;

    //Strong wrapper types
    struct STR_Parameter {
    };
    struct FUN_Parameter {
    };
    using FUN_index = NamedType<int, FUN_Parameter>;
    using STR_index = NamedType<int, STR_Parameter>;
    using Storage_FUN_index = NamedType<int, struct Storage_FUN_index_s>;
    using Storage_Var_index = NamedType<int, struct Storage_Var_index_s>;


    struct executor {

        friend class bfs_organizer;

    private:

        //Pointer to the organizer instance this executor is handled by
        //The organizer holds data like, Functors, Code, etc.
        bfs_organizer *organizer = nullptr;

        //Pointer to the executor from whom this executor emerged in the call instruction
        //Note: size_t::max represents that this exec has no parent -- see @func has_parent
        const executor* parent = nullptr;

        //Every child reuses the heap of the parent. So the child heaps build upon the parents heap. Thous
        //the child-heap starts at index parent-heap.size and ends at parent-heap.size + child-heap.size
        size_t heap_start_index = 0;

        //Every child may need to overwrite some parts of the heap of the parent. As many Childs rely on one heap
        //a particular child is not allowed to write in parent heap. So we need to store local changes
        std::unordered_map<size_t, heap_reg> changes_to_parent{};

        std::vector<heap_reg> heap{};

        std::vector<std::unique_ptr<executor>> children;

        wam::runtime_error runtime_error;

    public:
        //We also need to keep track whether this exec is from an original user entered query
        //the information is stored in solves_term_code.is_from_original_query()
        //whatever the exec solved the last is stored in term_codes.back()
        inline bool is_from_user_entered_query() const{
            assert(term_codes.size() == 1);
            return term_codes.back()->is_from_original_query();
        }

        std::vector<heap_reg_with_i> registers;

        mode read_or_write;

        size_t S;

        EXEC_STATE state = EXEC_STATE::RUNNING;

        //Used as a stack
        std::vector<compiled_atom*> term_codes;
        //Used as a stack
        std::vector<wam::environment> environments;

        executor& operator=(const executor & other)=default;
        executor& operator=(executor && other)=default;
        executor(const executor& other) = default;
        executor(executor&& other) = default;
        executor() = default;

        executor(size_t term_codes_size): term_codes{term_codes_size}{}

        inline std::vector<wam::heap_reg_with_i>& cur_permanent_registers(){
            assert(!environments.empty());
            return environments.back().permanent_registers;
        }

        inline bfs_organizer *get_organizer() const {
            return organizer;
        }

        const functor_view& functor_of(Storage_FUN_index storage_fun_index)const;
        functor_view& functor_of(Storage_FUN_index storage_fun_index);

        std::string var_name_of(Storage_Var_index var_index);
        inline functor_view &functor_of(STR_index STR_index){
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        inline const functor_view &functor_of(STR_index STR_index) const {
            return functor_of(FUN_index{heap_at(STR_index.get()).index});
        }

        functor_view &functor_of(FUN_index FUN_index);

        const functor_view &functor_of(FUN_index FUN_index) const;

        size_t storage_index_of(const functor_view &functor)const;

        inline void push_back(const heap_reg& regist){
            heap.push_back(regist);
        }
        inline void push_back_STR(){
            heap.emplace_back(heap_tag::STR,(int)  heap_size() + 1);
        }
        inline void push_back_FUN(int functor_index){
            heap.emplace_back(heap_tag::FUN, functor_index);
        }
        inline void push_back_EVAL_FUN(int eval_func_index){
            heap.emplace_back(heap_tag::EVAL_FUN, eval_func_index);
        }
        inline void push_back_unbound_REF(short var_index) {
            heap.emplace_back((int) heap_size(), var_index);
        }
        inline void push_back_cons(int constant_i){
            heap.emplace_back(heap_tag::CONS, constant_i);
        }
        inline void push_back_int(int value){
            heap.emplace_back(heap_tag::INT, value);
        }
        inline size_t heap_size()const{
            return heap_start_index + heap.size();
        }

        heap_reg heap_back()const;

        heap_reg& heap_modify(size_t index);

        heap_reg heap_at(size_t index)const;

        inline void move_from_parent(executor& parent){
            this->parent = &parent;
            heap_start_index = parent.heap_size();
            organizer = parent.organizer;

            if(!parent.environments.empty()){
                environments = std::move(parent.environments);
                parent.environments.clear();
                parent.environments.push_back(environments.back());
            }
            //Copy only as needed, but shrink to fit always to make sure, size is as small as possible
            parent.environments.shrink_to_fit();

            registers = parent.registers;

            term_codes = std::move(parent.term_codes);
            parent.term_codes.clear();
            parent.term_codes.push_back(term_codes.back());
            parent.term_codes.shrink_to_fit();
            term_codes.pop_back();

            changes_to_parent.reserve(5);
            heap.reserve(parent.heap.size());

            parent.state = EXEC_STATE::ARCHIVED;
        }

        inline void set_parent(const executor& parent){
            this->parent = &parent;
            heap_start_index = parent.heap_size();

            //Necessary copies - for now

            organizer = parent.organizer;
            environments = parent.environments;
            //TODO figure out whether the registers are only necessary to copy from
            //head func -> first body atom
            registers = parent.registers;

            //term_codes are more efficiently copied in call instruction

            //TODO Monitor different heuristics
            changes_to_parent.reserve(5);
            heap.reserve(parent.heap.size());
        }

        inline bool has_parent()const {
            return parent != nullptr;
        }

        inline const executor& get_parent()const{
            return *parent;
        }

        inline const std::vector<std::unique_ptr<executor>>& get_children()const{
            return children;
        }
        inline void push_back_child(executor&& child){
            children.push_back(std::make_unique<executor>(std::move(child)));
        }

        inline executor& get_last_child()const {
            assert(!children.empty());
            return *children.back();
        }

        /**
         * Clears all data from this executor which is no longer needed when saving
         */
        inline void clear(){
            term_codes.erase(term_codes.begin(), term_codes.end() -1);
            term_codes.shrink_to_fit();
            if(environments.size() > 1){
                environments.erase(environments.begin(), environments.end() -1);
            }
        }

        inline void set_state(EXEC_STATE s){
            state = s;
        }

        inline void set_archived(){
            state = EXEC_STATE ::ARCHIVED;
        }

        compiled_atom* get_cur_or_solved_term_code()const{
            assert(!term_codes.empty());
            return term_codes.back();
        }

        compiled_atom *get_current_term_code()const{
            return term_codes.back();
        }

        compiled_atom *get_solved_term_code()const {
            assert(term_codes.size() == 1);
            return term_codes.back();
        }

        void inline set_failed(){
            state = EXEC_STATE::FAIL;
        }

        bool inline failed()const{
            return state == EXEC_STATE::FAIL || error_occured();
        }

        bool is_leaf()const{
            return false;
        }

        /*
         * Checks whether this executor succeeded, and sets the internal state accordingly
         */
        bool inline check_success(){
            if(!failed() &&
                std::all_of(term_codes.rbegin() + 1,
                        term_codes.rend(),
                        [](const compiled_atom* term_code){return term_code->is_deallocate();})){
                state = EXEC_STATE::SUCCESS;
                return true;
            }
            return false;
        }

        bool inline succeeded() const{
            return state == EXEC_STATE::SUCCESS;
        }

        bool inline is_archived()const{
            return state == EXEC_STATE ::ARCHIVED;
        }

        bool inline is_running()const{
            return state == EXEC_STATE ::RUNNING;
        }

        bool inline error_occured()const{
            return state == EXEC_STATE ::RUNTIME_EXCEPTION_FAIL;
        }

        void inline set_runtime_error_flag(){
            set_state(EXEC_STATE::RUNTIME_EXCEPTION_FAIL);
        }

        void inline set_runtime_error(const wam::runtime_error& err){
            set_runtime_error_flag();
            runtime_error = err;
        }

        wam::runtime_error inline get_runtime_err()const{
            assert(error_occured());
            return runtime_error;
        }


    };
}


#endif //PROLOG_BFS_EXECUTOR_H
