//
// Created by leonhard on 27.07.19.
//

#ifndef PROLOG_BFS_FUNCTOR_VIEW_H
#define PROLOG_BFS_FUNCTOR_VIEW_H

#include <string>

namespace wam {
    struct functor_view {
        std::string name;
        int arity;

        functor_view() = default;

        functor_view(std::string name, int arity) : name{name}, arity{arity} {}

        inline bool is_list()const{
            return this->name == "[";
        }
        inline bool is_empty_list()const{
            return this->name == "[" && arity ==0;
        }

        inline bool is_constant()const{
            return arity ==0 && !is_empty_list();
        }
    };

    inline bool operator==(const functor_view &a, const functor_view &b) {
        return a.arity == b.arity && a.name == b.name;
    }


}
namespace std{
    template<>
    struct hash<wam::functor_view>{
        std::size_t operator()(const wam::functor_view& elem)const{
           return (std::hash<std::string>()(elem.name)) ^ (std::hash<int>()(elem.arity) << 1);
        }
    };
}



#endif //PROLOG_BFS_FUNCTOR_VIEW_H
