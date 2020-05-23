//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_QUERY_GRAMMAR_H
#define PROLOG_BFS_QUERY_GRAMMAR_H

//#define BOOST_SPIRIT_DEBUG
//#define BOOST_SPIRIT_ASSERT_EXCEPTION

#include "base_grammar.h"

namespace wam{

    namespace _query_grammar{
        using result_t = node;
    }

    namespace qi = boost::spirit::qi;

    template<typename Iterator, typename Skipper>
    struct query_grammar : public base_grammar<Iterator, node(), Skipper> {
        qi::rule<Iterator, wam::_query_grammar::result_t(), Skipper> query;

        using base = base_grammar<Iterator, _query_grammar::result_t(), Skipper>;
        query_grammar() : query_grammar::base_grammar(query) {
            namespace phoenix = boost::phoenix;

            query = (qi::eps > base::atom % ',' > qi::lit('.'))
                   [phoenix::bind(&make_to_query, phoenix::ref(qi::_val), qi::_1)];

            query.name("query");

            base::activate_error_handler(query);

#ifdef BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_NODES((query))
#endif
        }
    };
}
#endif //PROLOG_BFS_QUERY_GRAMMAR_H
