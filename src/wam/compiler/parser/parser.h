//
// Created by leonhard on 06.02.20.
//

#ifndef PROLOG_BFS_PARSER_H
#define PROLOG_BFS_PARSER_H


#include <string_view>
#include "query_grammar.h"
#include "program_grammar.h"
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace wam {

    typedef boost::spirit::line_pos_iterator<std::string_view::const_iterator> source_iterator;

    template<typename Result>
    void parse_query(const std::string_view query_code, Result &result) {
        source_iterator iter{query_code.begin()};
        source_iterator end{query_code.end()};

        typedef wam::query_grammar<source_iterator, boost::spirit::qi::space_type> query_grammar;
        query_grammar grammar{};

        bool r = phrase_parse(iter, end, grammar, boost::spirit::qi::space, result);

        if (!(r && iter == end)) {
            throw grammar.error;
        }
    }


    template<typename Result>
    void parse_program(const std::string_view program_code, Result &result) {
        source_iterator iter{program_code.begin()};
        source_iterator end{program_code.end()};

        typedef wam::program_grammar<source_iterator, boost::spirit::qi::space_type> prolog_grammar;
        prolog_grammar grammar{};
        bool r = phrase_parse(iter, end, grammar, boost::spirit::qi::space, result);

        if (!(r && iter == end)) {
            throw grammar.error;
        }
    }
}

#endif //PROLOG_BFS_PARSER_H
