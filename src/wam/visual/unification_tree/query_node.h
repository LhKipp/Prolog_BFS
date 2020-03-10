//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_QUERY_NODE_H
#define PROLOG_BFS_QUERY_NODE_H

#include "var_binding_node.h"
#include "../../data/term_code.h"
#include <memory>

namespace wam {
    class query_node{
    private:
        const term_code *query;
        std::unique_ptr<std::vector<var_binding_node>> children;

    public:
        query_node(){}
        query_node(const term_code *query,
                const size_t children_count) :
                query(query),
                children(std::make_unique<std::vector<var_binding_node>>(children_count)) {}

        /**
         * @return true if there is no fact with similar most outer functor as this query has,
         * and therefore the unification process failed.
         */
        bool failed()const{
            return children->empty();
        }

        /**
         *
         * @return all fact nodes with which a unification has been tried.
         */
        std::vector<var_binding_node> &getChildren(){
            return *children;
        }

        const std::vector<var_binding_node> &getChildren()const{
            return *children;
        }

        /**
         *
         * @return the query as string
         */
        const std::string& get_query_as_string() const{
            return query->get_code_info().value;
        }

        /**
         *
         * @return true if the query is from a user entered query.
         * False if this query is from a rule.
         */
        bool is_from_orig_query()const{
            return query->is_from_original_query();
        }

        /**
         *
         * @return the query code line (0 based), if this query is from a rule (program code).
         * Otherwise it throws an error.
         * Note: If the query stretches over multiple lines, the first line is returned
         */
        size_t get_query_code_line()const{
            assert(!is_from_orig_query());
            return query->get_code_info().line;
        }

    };
}

#endif //PROLOG_BFS_QUERY_NODE_H
