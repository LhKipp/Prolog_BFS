//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_BASE_GRAMMAR_H
#define PROLOG_BFS_BASE_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
#include <boost/phoenix/phoenix.hpp>

#include <wam/compiler/parser/parsed_helper_types/binary_arithmetic_predicate.h>
#include <wam/compiler/parser/parsed_helper_types/expressions.h>
#include "util/util.h"
#include "util/error_handler.h"
#include "parser_error.h"

BOOST_FUSION_ADAPT_STRUCT(
        node,
        (STORED_OBJECT_FLAG, type)
        (std::string, name)
)

namespace wam{
    namespace qi = boost::spirit::qi;


    template<typename Iterator, typename Base_Value, typename Skipper>
    struct base_grammar : qi::grammar<Iterator, Base_Value, Skipper> {
    public:
        error_handler<> handler;
        parser_error error;

        qi::rule<Iterator, node(), Skipper> built_in_pred;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_equals;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_not_equals;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_is;

        qi::rule<Iterator, node(), Skipper> constant;
        qi::rule<Iterator, node(), Skipper> functor;
        qi::rule<Iterator, std::string(), Skipper> functor_name;
        qi::rule<Iterator, node(), Skipper> list;
        qi::rule<Iterator, node(), Skipper> variable;
        qi::rule<Iterator, void(), Skipper> comment;
        qi::rule<Iterator, node(), Skipper> prolog_element;
        qi::rule<Iterator, node(), Skipper> atom;

        qi::rule<Iterator, node(), Skipper> expression;
        qi::rule<Iterator, parser::chained_value(), Skipper> chained_sum;
        qi::rule<Iterator, parser::chained_value(), Skipper> chained_prod;
        qi::rule<Iterator, parser::chained_value(), Skipper> chained_pow;
        qi::rule<Iterator, parser::chained_expr(), Skipper> sum;
        qi::rule<Iterator, node(), Skipper> product;
        qi::rule<Iterator, parser::chained_expr(), Skipper> product_helper;
        qi::rule<Iterator, node(), Skipper> power;
        qi::rule<Iterator, parser::chained_expr(), Skipper> power_helper;
        qi::rule<Iterator, node(), Skipper> value;
        qi::rule<Iterator, node(), Skipper> number;

        base_grammar(qi::rule<Iterator, Base_Value, Skipper> &start) : base_grammar::base_type(start) {
            namespace phoenix = boost::phoenix;
            namespace bf = boost::fusion;
            using qi::attr;
            using qi::_val;
            using qi::lit;
            using qi::string;
            using qi::lexeme;
            using qi::char_;
            using qi::on_error;
            using qi::fail;
            //Comments start with %
            comment = lexeme['%' > *(char_ - lit('\n')) > '\n'];

            constant = qi::attr(STORED_OBJECT_FLAG::CONSTANT) >> lexeme[char_("a-z") > *char_("a-zA-Z_0-9")];
            functor_name %= lexeme[char_("a-z") >> *(char_("a-zA-Z_0-9")) >> '('];
            functor = functor_name
                      [phoenix::bind(&make_to_func, qi::_1, phoenix::ref(_val))]
                      //Append each upcoming child to result node
                      > prolog_element
                        [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                        % ','
                      > ')';

            variable = attr(STORED_OBJECT_FLAG::VARIABLE) >> (lexeme[char_("A-Z") > *char_("a-zA-Z_0-9")]);

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

            prolog_element %= functor | constant | variable | list | number;

            using boost::spirit::repository::qi::iter_pos;
            atom = (iter_pos >> (built_in_pred | functor | constant) >> qi::no_skip[iter_pos])
            [phoenix::bind(&add_source_code_info<Iterator>, phoenix::ref(qi::_2), qi::_1, qi::_3),
             qi::_val = qi::_2];


            number = qi::attr(STORED_OBJECT_FLAG::INT) >> (lexeme[+char_("0-9")]);
            value = number | (lit('(') > expression > lit(')')) | variable;
            chained_pow = lit("^") > attr(STORED_OBJECT_FLAG::INT_POW) > value;
            power_helper = attr(STORED_OBJECT_FLAG::POWER) >> value >> -chained_pow;
            power = power_helper;
            chained_prod = (lit("*") > (attr(STORED_OBJECT_FLAG::MULT) > power))
                           |(lit("//") > (attr(STORED_OBJECT_FLAG::INT_DIV) > power));
            product_helper = attr(STORED_OBJECT_FLAG::PRODUCT) >> power >> *chained_prod;
            product = product_helper;
            chained_sum = (lit("+") > (attr(STORED_OBJECT_FLAG::PLUS) > product))
                            |(lit("-") > (attr(STORED_OBJECT_FLAG::MINUS) > product));
            sum = attr(STORED_OBJECT_FLAG::SUM) >> product >> *chained_sum;
            expression = sum;

            //Built in predicates definitions
            built_in_equals = prolog_element >> string("==") >> prolog_element;
            built_in_not_equals = prolog_element >> string("\\==") >> prolog_element;
            //Built in arithmetic binary predicates
            built_in_is = (number | variable) >> string("is") >> expression;
            built_in_pred = built_in_equals
                    | built_in_not_equals
                    | built_in_is;

            built_in_equals.name("==");
            built_in_not_equals.name("\\==");
            comment.name("comment");
            variable.name("variable");
            functor.name("functor");
            list.name("list");
            constant.name("constant");
            prolog_element.name("prolog_element");

            expression.name("Expression");
            sum.name("sum");
            product.name("product");
            power.name("power");
            value.name("value");
            number.name("integer");



#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES(
                    (comment)(variable)(functor)(list)(constant)(prolog_element)
                    (expression)(sum)(product)(power)(value)(number)
                    (built_in_pred)(built_in_equals)(built_in_not_equals)
                    )
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
