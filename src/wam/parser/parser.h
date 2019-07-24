//
// Created by leonhard on 05.07.19.
//

#ifndef PROLOG_BFS_PARSER_H
#define PROLOG_BFS_PARSER_H


#include <string>
#include <stack>
#include <unordered_map>
#include <functional>
#include <forward_list>
#include<vector>
#include "../../prolog/data/term.h"

namespace prolog {

    struct node {
        STORED_OBJECT_FLAG type;
        std::string name;
        std::vector<node> children;
    };

    class parser {

        /*
         * Parses a query term e.G. p(Z,h(Z,W),f(W))
         */
        void parse_query(const std::string &line) {
            node pred;
            //reading the name of the query predicate
            size_t name_end = line.find_first_of('(');

            if (name_end == std::string::npos) {
                pred.type = CONSTANT;
                std::copy(line.cbegin(), line.cend(), pred.name.begin());
            } else {
                pred.type = FUNCTOR;
                std::copy(line.cbegin(), line.cbegin() + name_end, pred.name.begin());
            }

            //holds the current parent node on top.
            std::stack<node*> current_parent;
            //At the begin, its the outer pred
            current_parent.push(&pred);

            size_t last_pos = name_end + 1;
            while (last_pos < line.size()) {
                node inner_term;
                //If the inner term starts Capital it is a variable
                if(std::isupper(line[last_pos])){
                    inner_term.type = VARIABLE;
                    size_t inner_term_end = line.find_first_of(",)", last_pos);
                    std::copy(line.cbegin() + last_pos, line.cbegin() + inner_term_end, inner_term.name.begin());
                    //Assign the inner_term as a child of the current parent
                    current_parent.top()->children.push_back(inner_term);

                    //if inner_term ended through ) then the current functor is ended => pop it from stack
                    if(line[inner_term_end] == ')'){current_parent.pop();}
                    last_pos= inner_term_end + 1;
                }else{//Inner term is either functor or constant
                    auto inner_term_end = line.find_first_of(",)", last_pos);
                    auto opening_brack_pos = line.find_first_of('(', last_pos);
                    //If there is no ( or ,) is before ( the current inner_term is a constant
                    if(opening_brack_pos == std::string::npos || inner_term_end < opening_brack_pos){
                        inner_term.type = CONSTANT;
                        std::copy(line.cbegin() + last_pos, line.cbegin() + inner_term_end, inner_term.name.begin());
                        current_parent.top()->children.push_back(inner_term);

                        if(line[inner_term_end] == ')'){current_parent.pop();}
                        last_pos= inner_term_end + 1;
                    }else{//inner_term is a functor
                        inner_term.type = FUNCTOR;
                        std::copy(line.cbegin() + last_pos, line.cbegin() + opening_brack_pos, inner_term.name.begin());
                        //Assign the functor as a child of the parent and make the functor the current parent
                        current_parent.top()->children.push_back(inner_term);
                        current_parent.push(&current_parent.top()->children.back());

                        last_pos = opening_brack_pos + 1;
                    }
                }
            }

            std::vector<node> innerTerms;


        }

        /*
         *
         */
        std::unordered_multimap<std::string, std::forward_list<std::function<void(void)>>>
        parse(const std::vector<std::string> &lines) {
        }


    };
}


#endif //PROLOG_BFS_PARSER_H
