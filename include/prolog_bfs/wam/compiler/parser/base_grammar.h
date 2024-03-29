//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_BASE_GRAMMAR_H
#define PROLOG_BFS_BASE_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>
#include <boost/phoenix/phoenix.hpp>

#include "prolog_bfs/wam/compiler/parser/parsed_helper_types/binary_arithmetic_predicate.h"
#include "prolog_bfs/wam/compiler/parser/parsed_helper_types/expressions.h"
#include "prolog_bfs/wam/compiler/parser/parsed_helper_types/prolog_data_types.h"
#include "prolog_bfs/wam/compiler/error/compiler_error.h"
#include "prolog_bfs/wam/compiler/parser/util/parser_util.h"
#include "prolog_bfs/wam/compiler/parser/util/error_handler.h"

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
        compiler::error error;

        qi::rule<Iterator, node(), Skipper> built_in_pred;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_equals;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_not_equals;
        qi::rule<Iterator, parser::binary_arithmetic_predicate(), Skipper> built_in_is;

        qi::rule<Iterator, node(), Skipper> constant;
        qi::rule<Iterator, node(), Skipper> functor;
        qi::rule<Iterator, parser::functor(), Skipper> functor_helper;
        qi::rule<Iterator, parser::functor(), Skipper> empty_list_t;
        qi::rule<Iterator, node(), Skipper> empty_list;
        qi::rule<Iterator, parser::normal_list(), Skipper> normal_list_t;
        qi::rule<Iterator, node(), Skipper> normal_list;
        qi::rule<Iterator, parser::finished_list(), Skipper> finished_list_t;
        qi::rule<Iterator, node(), Skipper> finished_list;
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

            functor_helper = lexeme[char_("a-z") >> *(char_("a-zA-Z_0-9")) >> '(']
                      > prolog_element
                      % ','
                      > lit(')');
            functor = functor_helper;
            constant = attr(STORED_OBJECT_FLAG::CONSTANT) >> lexeme[char_("a-z") > *char_("a-zA-Z_0-9")];
            variable = attr(STORED_OBJECT_FLAG::VARIABLE) >> (lexeme[char_("A-Z") > *char_("a-zA-Z_0-9")]);
            number = attr(STORED_OBJECT_FLAG::INT) >> (lexeme[-char_("-") >> +char_("0-9")]);

            //Every list is of type: list(X,Xs) or nil
            //Where X is an value (const, func, list) and Xs is the rest list
            //nil is represented as an empty list (list node without children)
            empty_list_t = string("[") >> "]" >> attr(std::vector<node>());
            empty_list = empty_list_t;
            normal_list_t = lit("[") >> prolog_element % ',' >> lit("]");
            finished_list_t = "[" >> prolog_element % ',' >> '|' >> (variable | list) >> "]";
            normal_list = normal_list_t;
            finished_list = finished_list_t;
            list = empty_list | normal_list | finished_list;

            prolog_element %= functor | constant | list | expression;

            using boost::spirit::repository::qi::iter_pos;
            atom = (iter_pos >> (built_in_pred | functor | constant) >> qi::no_skip[iter_pos])
            [phoenix::bind(&add_source_code_info<Iterator>, phoenix::ref(qi::_2), qi::_1, qi::_3),
             qi::_val = qi::_2];


            value = variable | number | (lit('(') > expression > lit(')'));
            chained_pow = char_("^") > value;
            power_helper = value >> -chained_pow;
            power = power_helper;
            chained_prod = (char_("*") > power)
                           |(qi::string("//") > power);
            product_helper = power >> *chained_prod;
            product = product_helper;
            chained_sum = (char_("+") > product)
                            | (char_("-") > product);
            sum = product >> *chained_sum;
            expression = sum;

            //Built in predicates definitions
            built_in_equals = prolog_element >> string("==") >> prolog_element;
            built_in_not_equals = prolog_element >> string("\\==") >> prolog_element;
            //Built in arithmetic binary predicates
            built_in_is = prolog_element >> string("is") >> expression;
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
            chained_sum.name("sum");
            product.name("product");
            product_helper.name("product");
            chained_prod.name("product");
            power.name("power");
            power_helper.name("power");
            chained_pow.name("power");
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
