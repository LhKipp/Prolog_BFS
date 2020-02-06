//
// Created by leonhard on 04.08.19.
//

#ifndef PROLOG_BFS_SEEN_REGISTER_H
#define PROLOG_BFS_SEEN_REGISTER_H

#include <cstddef>
namespace wam::helper {
    enum class register_type {
        Y_REG,
        A_REG,
        X_REG,
        NONE
    };

    struct seen_register {
        register_type type;
        size_t index;

        seen_register()=default;

        seen_register(register_type type, size_t index) : type(type), index(index) {}

        seen_register(const seen_register& other) = default;
        seen_register(seen_register&& other) = default;
        seen_register& operator=(const seen_register&)=default;
        seen_register& operator=(seen_register&&)=default;

        bool is_a_reg()const{
            return type == register_type ::A_REG;
        }
        bool is_x_reg()const{
            return type == register_type ::X_REG;
        }

        bool operator==(const seen_register &rhs) const {
            return type == rhs.type &&
                   index == rhs.index;
        }

        bool operator!=(const seen_register &rhs) const {
            return !(rhs == *this);
        }
    };

}
namespace std{
    template<>
    struct hash<wam::helper::seen_register>{
        std::size_t operator()(const wam::helper::seen_register& elem)const{
            return (std::hash<std::size_t>{}(static_cast<std::size_t>(elem.type)) ^ (std::hash<std::size_t>()(elem.index) << 1));
        }
    };
}
#endif //PROLOG_BFS_SEEN_REGISTER_H
