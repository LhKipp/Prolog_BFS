//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_QUERY_NODE_H
#define PROLOG_BFS_QUERY_NODE_H

#include "var_binding_node.h"
#include "../../executor/executor.h"
#include "../../data/compiled_atom.h"
#include <memory>

namespace wam {
    class query_node{
    private:
        const executor *exec;

        std::unique_ptr<std::vector<var_binding_node>> children;
        
        int _id;

        node query_name;
        std::string resolved_query_name;

    public:
        query_node() = default;

        void clear_memory(){
            children.reset();
            query_name = node();
            resolved_query_name = "";
        }

        query_node(const executor *query,
                const size_t children_count,
                const int node_id) :
                _id(node_id),
                exec(query),
                children(std::make_unique<std::vector<var_binding_node>>(children_count)) {
        }

        query_node(const query_node& other) {
            resolved_query_name = other.resolved_query_name;
            _id = other._id;
            exec = other.exec;
            if(other.children){
                children = std::make_unique<std::vector<var_binding_node>>(*other.children);
            }
        }
        query_node& operator=(const query_node& other) {
            resolved_query_name = other.resolved_query_name;
            _id = other._id;
            exec = other.exec;
            if (other.children) {
                children = std::make_unique<std::vector<var_binding_node>>(*other.children);
            }
            return *this;
        }

        int get_node_id() {
            return _id;
        }

        bool is_oom_node()const{
            return exec == nullptr && _id == 0 && children->empty();
        }

        /**
         * @return true if there is no fact with similar most outer functor as this query has,
         * and therefore the unification process failed.
         */
        bool failed()const{
            return exec->failed();
        }

        bool is_to_be_continued()const{
            return exec->is_running();
        }

        /**
         *
         * @return all fact nodes with which a unification has been tried.
         */
        std::vector<var_binding_node> &get_children(){
            return *children;
        }

        const std::vector<var_binding_node> &get_children()const{
            return *children;
        }
        
        /*
        * emscripten isn't able to compile with the two above overloaded functions
        * and select_overload also doesn't work because of const overloads seem to
        * be not supported. Created this extra method so there is no overload anymore.
        */
        const std::vector<var_binding_node> &get_children_wasm()const{
            return *children;
        }

        const std::string& get_unresolved_query_as_str()const{
            return get_atom().get_code_info().value;
        }
        /**
         *
         * @return the resolved query as string
         */
        const std::string& get_resolved_query_as_str() const{
            return resolved_query_name;
        }

        /**
         *
         * @return the query code line_begin, if this query is from a rule (program code).
         * Otherwise it throws an error.
         * Note: If the query stretches over multiple lines, the first line_begin is returned
         */
        size_t get_query_code_line()const{
            return get_atom().get_code_info().line_begin;
        }

        const compiled_atom& get_atom()const{
            return *exec->get_cur_or_solved_term_code();
        }

        const executor& get_exec()const{
            return *exec;
        }

        void set_name(node name){
            query_name = std::move(name);
            resolved_query_name = query_name.to_string();
        }

        void set_resolved_name(std::string resolved_name){
            this->resolved_query_name = std::move(resolved_name);
        }

        const node& get_name()const{
            return query_name;
        }
    };
}

#endif //PROLOG_BFS_QUERY_NODE_H
