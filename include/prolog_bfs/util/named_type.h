//
// Created by leonhard on 30.07.19.
//

#ifndef PROLOG_BFS_NAMED_TYPE_H
#define PROLOG_BFS_NAMED_TYPE_H

#include <utility>

template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};
#endif //PROLOG_BFS_NAMED_TYPE_H
