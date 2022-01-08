//
// Created by leonhard on 08.05.20.
//

#include <catch2/catch.hpp>
#include "prolog_bfs/wam/compiler/parser/query_grammar.h"
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <string_view>

template<typename Iterator, typename Skipper>
struct test_grammar : public base_grammar<Iterator, node(), Skipper> {
    qi::rule<Iterator, node(), Skipper> query;
    using base = base_grammar<Iterator, node(), Skipper>;
    test_grammar() : test_grammar::base_grammar(query) {
        query = (qi::eps > base::atom > qi::lit('.'));
        query.name("query");

#ifdef BOOST_SPIRIT_DEBUG
        BOOST_SPIRIT_DEBUG_NODES((query))
#endif
    }
};

typedef boost::spirit::line_pos_iterator<std::string_view::const_iterator> source_iterator;
typedef test_grammar<source_iterator, boost::spirit::qi::space_type> tgrm;
TEST_CASE("cmpiler"){
    std::string q = "X is 3.";
    const std::string_view query_code(q);
    source_iterator b(query_code.begin());
    source_iterator e(query_code.end());
    node result;

    tgrm t{};
    bool r = phrase_parse(b, e, t, boost::spirit::qi::space, result);
    int i = 3;
    REQUIRE(r);

}
