//
// Created by leonhard on 06.03.20.
//

#ifndef PROLOG_BFS_PARSER_ERROR_H
#define PROLOG_BFS_PARSER_ERROR_H


#include <string>

namespace wam{
    class parser_error {
    public:

        parser_error(): _exists(false){}
        parser_error(const std::string &expected, int onRow, int onCol) : expected(expected), on_row(onRow),
                                                                          on_col(onCol), _exists(true){}

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

        bool exists() const{
            return _exists;
        }

        void set_exists(bool exists){
            _exists = exists;
        }

    private:
        bool _exists;
        std::string expected;
        int on_row;
        int on_col;
    };

}


#endif //PROLOG_BFS_PARSER_ERROR_H
