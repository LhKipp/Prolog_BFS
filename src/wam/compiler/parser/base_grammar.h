//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_BASE_GRAMMAR_H
#define PROLOG_BFS_BASE_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
#include <boost/phoenix/phoenix.hpp>
#include "../util/node.h"
#include "util/util.h"
#include "util/error_handler.h"
#include "parser_error.h"


namespace wam{
    namespace qi = boost::spirit::qi;

    template<typename Iterator, typename Base_Value, typename Skipper>
    struct base_grammar : qi::grammar<Iterator, Base_Value, Skipper> {
    public:

        error_handler<> handler;
        parser_error error;

        qi::rule<Iterator, node(), Skipper> constant;
        qi::rule<Iterator, std::string(), Skipper> constant_name;
        qi::rule<Iterator, node(), Skipper> functor;
        qi::rule<Iterator, std::string(), Skipper> functor_name;
        qi::rule<Iterator, node(), Skipper> list;
        qi::rule<Iterator, node(), Skipper> variable;
        qi::rule<Iterator, std::string(), Skipper> variable_name;
        qi::rule<Iterator, void(), Skipper> comment;
        qi::rule<Iterator, node(), Skipper> prolog_element;
        qi::rule<Iterator, node, Skipper> atom;

        base_grammar(qi::rule<Iterator, Base_Value, Skipper> &start) : base_grammar::base_type(start) {
            namespace phoenix = boost::phoenix;
            using qi::_val;
            using qi::lit;
            using qi::lexeme;
            using qi::char_;
            using qi::on_error;
            using qi::fail;
            //Comments start with %
            comment = lexeme['%' > *(char_ - lit('\n')) > '\n'];

            constant_name %= (lexeme[char_("a-z") > *char_("a-zA-Z_0-9")]);
            constant = (constant_name)
            [phoenix::bind(&make_to_cons, qi::_1, phoenix::ref(_val))];

            functor_name %= lexeme[char_("a-z") >> *(char_("a-zA-Z_0-9")) >> '('];
            functor = functor_name
                      [phoenix::bind(&make_to_func, qi::_1, phoenix::ref(_val))]
                      //Append each upcoming child to result node
                      > prolog_element
                        [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                        % ','
                      > ')';

            variable_name %= (lexeme[char_("A-Z") > *char_("a-zA-Z_0-9")]);
            variable = variable_name
            [phoenix::bind(&make_to_var, qi::_1, phoenix::ref(_val))];

            //Every list is of type: list(X,Xs) or nil
            //Where X is an value (const, func, list) and Xs is the rest list
            //nil is represented as an empty list (list node without children)
            list = char_('[')
                   [phoenix::bind(&make_to_list, qi::_1, phoenix::ref(_val))]
                   //Append each upcoming list element to results children
                   > (
                           (prolog_element
                            [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                            % ','
                            //Every list may end with concatenation of rest list.
                            //Rest list may be actual list or var
                            > -(char_('|')
                                [phoenix::bind(&set_list_finished, qi::_1, phoenix::ref(_val))]
                                > (variable | list)
                                [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                           )
                            > char_(']')
                            [phoenix::bind(&childs_to_list, phoenix::ref(_val), qi::_1)]
                           ) | char_(']'));

            prolog_element %= functor | constant | variable | list;

            using boost::spirit::repository::qi::iter_pos;
            //TODO the second iter_pos points before the next non skipped char.
            //So that the range [it1, it2[ may include blanks.
            //Clean the code up somehow, so that the sec iter_pos points to end of func/cons
            atom = (iter_pos >> (functor | constant) >> iter_pos)
            [phoenix::bind(&add_source_code_info<Iterator>, phoenix::ref(qi::_2), qi::_1, qi::_3),
             qi::_val = qi::_2];

            comment.name("comment");
            variable.name("variable");
            functor.name("functor");
            list.name("list");
            constant.name("constant");
            prolog_element.name("prolog_element");


#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((comment)(variable)(functor)(list)(constant)(prolog_element))
#endif
        }

        void inline activate_error_handler(qi::rule<Iterator, Base_Value, Skipper> &rule){
            namespace phoenix = boost::phoenix;
            qi::on_error<qi::fail>(rule,
                                   phoenix::bind(phoenix::ref(handler),
                                                 phoenix::ref(error),
                                                 qi::_1,
                                                 qi::_2,
                                                 qi::_3,
                                                 qi::_4)
            );
        }
    };
}

#endif //PROLOG_BFS_BASE_GRAMMAR_H
