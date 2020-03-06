//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_PARSER_ERROR_H
#define PROLOG_BFS_PARSER_ERROR_H


#include <string>

namespace wam{
    class parser_error {
    public:
        std::string &get_expected(){
            return expected;
        }
        const std::string &get_expected() const {
            return expected;
        }

        void set_expected(const std::string &expected) {
            parser_error::expected = expected;
        }

        int get_row() const {
            return on_row;
        }

        void set_row(int onRow) {
            on_row = onRow;
        }

        int get_col() const {
            return on_col;
        }

        void set_col(int onCol) {
            on_col = onCol;
        }

        bool is_valid(){
            return on_col !=0 && on_row !=0;
        }

    private:
        std::string expected;
        int on_row;
        int on_col;
    };

}


#endif //PROLOG_BFS_PARSER_ERROR_H
