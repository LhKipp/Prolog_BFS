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

    namespace qi = boost::spirit::qi;

    template<typename Iterator, typename Skipper>
    struct program_grammar : public base_grammar<Iterator, std::vector<boost::optional<node>>(), Skipper> {
        using base = base_grammar<Iterator, std::vector<boost::optional<node>>(), Skipper>;

        qi::rule<Iterator, std::vector<boost::optional<node>>(), Skipper> program;
        qi::rule<Iterator, node(), Skipper> clause;

        program_grammar() : program_grammar::base_grammar(program) {

            namespace phoenix = boost::phoenix;
            using boost::spirit::qi::_val;
            using boost::spirit::qi::lit;
            using boost::spirit::qi::lexeme;
            using qi::char_;
            using qi::on_error;
            using qi::fail;
            //A program is either a clause or comments
            program %= *(clause | base::comment);
            //clause = head :- body1, body2... whereas head and body are only allowed to be cons or func
            clause = qi::eps
                     [phoenix::bind(&make_to_top_node, qi::_1, phoenix::ref(_val))]
                             >> (base::functor | base::constant)
                             [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                     > -(":-" >> (base::functor | base::constant)
                                 [phoenix::bind(&node::add_to_children, phoenix::ref(_val), qi::_1)]
                                 % ',') > '.' > -base::comment;

            clause.name("clause");
            program.name("program");

            base::activate_error_handler(program);

#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((program)(clause))
#endif
        }
    };
}
#endif //PROLOG_BFS_PROGRAM_GRAMMAR_H
