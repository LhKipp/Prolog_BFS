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

        const term_code *get_query() const{
            return query;
        }

        void set_query(const term_code *query){
            this->query = query;
        }

        std::vector<var_binding_node> &getChildren(){
            return *children;
        }
    };
}

#endif //PROLOG_BFS_QUERY_NODE_H
