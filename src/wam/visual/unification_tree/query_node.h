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

        std::string resolved_query_name;

    public:
        query_node() = default;

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

        /**
         *
         * @return the query as string
         */
        const std::string& get_query_as_str() const{
            return resolved_query_name;
        }

        void set_resolved_name(std::string name){
            resolved_query_name = std::move(name);
        }

        /**
         *
         * @return the query code line_begin (0 based), if this query is from a rule (program code).
         * Otherwise it throws an error.
         * Note: If the query stretches over multiple lines, the first line_begin is returned
         */
        size_t get_query_code_line()const{
            assert(!exec->is_from_user_entered_query());
            return get_atom().get_code_info().line_begin;
        }

        const compiled_atom& get_atom()const{
            return *exec->get_cur_or_solved_term_code();
        }
    };
}

#endif //PROLOG_BFS_QUERY_NODE_H
