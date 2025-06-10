#ifndef PAIR_H
#define PAIR_H

#include <bits/move.h>

template <typename Type1,typename Type2>
class Pair{

public:
    Type1 first;
    Type2 second;
    // 构造函数
    Pair()=default;
    // 通用构造函数，支持完美转发
    template<typename U1,typename U2>
    Pair(U1&& f, U2&& s) : first(std::forward<U1>(f)), second(std::forward<U2>(s)) {}
    // 拷贝构造
    Pair(const Pair &other)=default;
    // 移动构造
    Pair(Pair &&other) noexcept:first(std::move(other.first)),second(std::move(other.second)){}

    // 赋值函数
    // 拷贝赋值
    Pair &operator=(const Pair &other);
    // 移动赋值
    Pair &operator=(Pair &&other) noexcept;

    // 比较
    bool operator==(const Pair &other) const;
    bool operator!=(const Pair &other) const;
    bool operator<(const Pair &other) const;
    bool operator<=(const Pair &other) const;
    bool operator>(const Pair &other) const;
    bool operator>=(const Pair &other) const;
};

// 实现部分
template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator==(const Pair &other) const {
    return first == other.first && second == other.second;
}

template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator!=(const Pair &other) const {
    return !(*this == other);
}

template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator<(const Pair &other) const {
    if (first < other.first) return true;
    if (other.first < first) return false;
    return second < other.second;
}

template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator<=(const Pair &other) const {
    return !(other < *this);
}

template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator>(const Pair &other) const {
    return other < *this;
}

template <typename Type1, typename Type2>
bool Pair<Type1, Type2>::operator>=(const Pair &other) const {
    return !(*this < other);
}


// 默认拷贝赋值运算符
template <typename Type1, typename Type2>
Pair<Type1, Type2>& Pair<Type1, Type2>::operator=(const Pair &other) = default;

// 移动赋值运算符
template <typename Type1, typename Type2>
Pair<Type1, Type2>& Pair<Type1, Type2>::operator=(Pair &&other) noexcept = default;

#endif // PAIR_H