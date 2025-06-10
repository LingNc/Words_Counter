#ifndef SQLIST_H
#define SQLIST_H

#include <cstddef>
#include <vector>
#include <string>

template <typename Type>
class SqList{
    std::vector<Type> _data;
public:
    // 基本函数
    SqList()=default;
    SqList(size_t n) {
        _data.resize(n);
    }
    ~SqList()=default;
    
    // 调整大小
    void resize(size_t n) {
        _data.resize(n);
    }
    
    // 预留空间
    void reserve(size_t n) {
        _data.reserve(n);
    }

    // 元素操作
    void push_back(const Type &value);
    void push_back(Type &&value);
    void pop_back();

    // 元素访问
    Type &front();
    const Type &front() const;
    Type &back();
    const Type &back() const;
    Type &operator[](size_t index);
    const Type &operator[](size_t index) const;

    // 容量
    size_t size() const;
    bool empty() const;
    void clear();

    // 迭代器
    using iterator = typename std::vector<Type>::iterator;
    using const_iterator = typename std::vector<Type>::const_iterator;
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
};

// 元素操作
template <typename Type>
void SqList<Type>::push_back(const Type &value){
    _data.push_back(value);
}

template <typename Type>
void SqList<Type>::push_back(Type &&value){
    _data.push_back(std::move(value));
}

template <typename Type>
void SqList<Type>::pop_back(){
    if(!_data.empty()){
        _data.pop_back();
    }
}

// 元素访问
template <typename Type>
Type &SqList<Type>::front(){
    return _data.front();
}

template <typename Type>
const Type &SqList<Type>::front() const{
    return _data.front();
}

template <typename Type>
Type &SqList<Type>::back(){
    return _data.back();
}

template <typename Type>
const Type &SqList<Type>::back() const{
    return _data.back();
}

template <typename Type>
Type &SqList<Type>::operator[](size_t index){
    return _data[index];
}

template <typename Type>
const Type &SqList<Type>::operator[](size_t index) const{
    return _data[index];
}

// 容量
template <typename Type>
size_t SqList<Type>::size() const{
    return _data.size();
}

template <typename Type>
bool SqList<Type>::empty() const{
    return _data.empty();
}

template <typename Type>
void SqList<Type>::clear(){
    _data.clear();
}

// 迭代器
template <typename Type>
typename SqList<Type>::iterator SqList<Type>::begin(){
    return _data.begin();
}

template <typename Type>
typename SqList<Type>::const_iterator SqList<Type>::begin() const{
    return _data.begin();
}

template <typename Type>
typename SqList<Type>::iterator SqList<Type>::end(){
    return _data.end();
}

template <typename Type>
typename SqList<Type>::const_iterator SqList<Type>::end() const{
    return _data.end();
}

#endif // SQLIST_H