//
// Created by leonhard on 11.12.19.
//

#ifndef PROLOG_BFS_RANGE_H
#define PROLOG_BFS_RANGE_H

#include <cstddef>

namespace math {
    class inclusive_range {
    private:
        size_t lower;
        size_t upper;

    public:
        inclusive_range(int lower, int upper) : lower(lower), upper(upper) {}

        bool inline is_in(size_t number) const {
            return lower <= number && number <= upper;
        }

        size_t inline lower_bound()const{
            return lower;
        }
        size_t inline upper_bound()const{
            return upper;
        }

    };
}


#endif //PROLOG_BFS_RANGE_H
