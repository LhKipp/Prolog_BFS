//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_QUERY_GRAMMAR_H
#define PROLOG_BFS_QUERY_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include "../util/node.h"
#include "util/util.h"
#include "base_grammar.h"

namespace wam{
    namespace qi = boost::spirit::qi;
    template<typename Iterator, typename Skipper>
struct query_grammar : public base_grammar<Iterator, std::vector<node>(), Skipper> {
        qi::rule<Iterator, std::vector<node>(), Skipper> query;

        query_grammar() : query_grammar::base_grammar(query) {

            using base = base_grammar<Iterator, std::vector<node>(), Skipper>;
            using qi::lit;

            query %= qi::eps > (base::functor | base::constant) % ',' > -lit('.');

            query.name("query");

            base::activate_error_handler(query);

#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((query))
#endif
        }
    };
}
#endif //PROLOG_BFS_QUERY_GRAMMAR_H
