//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_PROGRAM_GRAMMAR_H
#define PROLOG_BFS_PROGRAM_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include "../util/node.h"
#include "util/util.h"
#include "parser_error.h"
#include "base_grammar.h"

namespace wam {

    namespace _program_grammar{
        using result_t = std::vector<boost::optional<node>>;
    }
    namespace qi = boost::spirit::qi;

    template<typename Iterator, typename Skipper>
    struct program_grammar : public base_grammar<Iterator, std::vector<boost::optional<node>>(), Skipper> {

        using base = base_grammar<Iterator, _program_grammar::result_t(), Skipper>;

        qi::rule<Iterator, _program_grammar::result_t(), Skipper> program;
        qi::rule<Iterator, node(), Skipper> rule;
        qi::rule<Iterator, std::vector<node>(), Skipper> clause;

        program_grammar() : program_grammar::base_grammar(program) {

            namespace phoenix = boost::phoenix;
            using boost::spirit::qi::_val;
            using boost::spirit::qi::lit;
            using boost::spirit::qi::lexeme;
            using qi::char_;
            using qi::on_error;
            using qi::fail;

            program %= qi::eps > *(base::comment | rule) > qi::eoi ;

            using boost::spirit::repository::qi::iter_pos;
            rule = (iter_pos >> clause >> qi::no_skip[iter_pos] > -base::comment)
            [phoenix::bind(&add_source_code_info<Iterator>, phoenix::ref(qi::_val), qi::_1, qi::_3),
             phoenix::bind(&make_to_children, phoenix::ref(qi::_val), qi::_2)];

            //clause = head :- body1, body2... whereas head and body are only allowed to be cons or func
            clause = base::atom
                     > -(":-" >> base::atom % ',') > '.';

            clause.name("clause");
            rule.name("rule");
            program.name("program");

            base::activate_error_handler(program);

#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((program)(clause))
#endif
        }
    };
}
#endif //PROLOG_BFS_PROGRAM_GRAMMAR_H
