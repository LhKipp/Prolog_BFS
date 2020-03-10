//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_QUERY_GRAMMAR_H
#define PROLOG_BFS_QUERY_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include "../util/node.h"
#include "util.h"
#include "base_grammar.h"

namespace qi = boost::spirit::qi;
namespace wam{
    template<typename Iterator, typename Skipper>
    struct query_grammar : qi::grammar<Iterator, std::vector<node>(), Skipper> {
        qi::rule<Iterator, std::vector<node>(), Skipper> query;
        qi::rule<Iterator, node(), Skipper> constant;
        qi::rule<Iterator, std::string(), Skipper> constant_name;
        qi::rule<Iterator, node(), Skipper> functor;
        qi::rule<Iterator, std::string(), Skipper> functor_name;
        qi::rule<Iterator, node(), Skipper> list;
        qi::rule<Iterator, node(), Skipper> variable;
        qi::rule<Iterator, std::string(), Skipper> variable_name;
        qi::rule<Iterator, node(), Skipper> prolog_element;

        explicit query_grammar() :
                query_grammar::base_type(query, "query") {
            namespace phoenix = boost::phoenix;
            using boost::spirit::qi::_val;
            using boost::spirit::qi::lit;
            using boost::spirit::qi::lexeme;
            using qi::char_;
            using qi::on_error;
            using qi::fail;

            query %= qi::eps > (functor | constant) % ',' > -lit('.');

            constant_name %= (lexeme[char_("a-z") > *char_("a-zA-Z_0-9")]);
            constant = (constant_name)
            [phoenix::bind(&wam::make_to_cons, qi::_1, phoenix::ref(_val))];

            functor_name %= lexeme[char_("a-z") >> *(char_("a-zA-Z_0-9")) >> '('];
            functor = functor_name
                      [phoenix::bind(&wam::make_to_func, qi::_1, phoenix::ref(_val))]
                      //Append each upcoming child to result node
                      > prolog_element
                        [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                        % ','
                      > ')';

            variable_name %= (lexeme[char_("A-Z") > *char_("a-zA-Z_0-9")]);
            variable = variable_name
            [phoenix::bind(&wam::make_to_var, qi::_1, phoenix::ref(_val))];

            //Every list is of type: list(X,Xs) or nil
            //Where X is an value (const, func, list) and Xs is the rest list
            //nil is represented as an empty list (list node without children)
            list = char_('[')
                   [phoenix::bind(&wam::make_to_list, qi::_1, phoenix::ref(_val))]
                   //Append each upcoming list element to results children
                   > (
                           (prolog_element
                            [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                            % ','
                            //Every list may end with concatenation of rest list.
                            //Rest list may be actual list or var
                            > -(char_('|')
                                [phoenix::bind(&wam::set_list_finished, qi::_1, phoenix::ref(_val))]
                                > (variable | list)
                                [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                           )
                            > char_(']')
                            [phoenix::bind(&wam::childs_to_list, phoenix::ref(_val), qi::_1)]
                           ) | char_(']'));

            prolog_element %= functor | constant | variable | list;

            variable.name("variable");
            functor.name("functor");
            list.name("list");
            constant.name("constant");
            prolog_element.name("prolog_element");
            query.name("query");

#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((query)(variable)(functor)(list)(constant)(prolog_element))
#endif

            namespace phoenix = boost::phoenix;
            qi::on_error<qi::fail>(query,
                                   phoenix::bind(phoenix::ref(handler),
                                                 phoenix::ref(error),
                                                 qi::_1,
                                                 qi::_2,
                                                 qi::_3,
                                                 qi::_4)
            );
        }

        error_handler<> handler;
    public:
        parser_error error;
    };
}
#endif //PROLOG_BFS_QUERY_GRAMMAR_H
