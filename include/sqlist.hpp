#ifndef SQLIST_H
#define SQLIST_H

#include <cstddef>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <string>

template <typename Type>
class SqList{
public:
    using iterator=Type *;
    using const_iterator=const Type *;
private:
    std::vector<Type> _data_v;
    size_t _capacity=0; // 容量
    size_t _size=0;
    // iterator
public:
    // 基本函数
    SqList()=default;
    explicit SqList(size_t n);

    // 初始化列表构造函数
    SqList(std::initializer_list<Type> initList):_data_v(initList){}
    ~SqList()=default;

    // 预留空间
    void reserve(size_t n);

    // 元素操作
    void push_back(const Type &value);
    void push_back(Type &&value);
    void pop_back();
    void insert(size_t index,const Type &value);
    void insert(const_iterator iter,const Type &value);

    // 元素访问
    Type &front();
    const Type &front() const;
    Type &back();
    const Type &back() const;
    Type &operator[](size_t index);
    const Type &operator[](size_t index) const;

    // 容量
    size_t size() const;
    size_t capacity() const;
    void resize(size_t newSize);
    bool empty() const;
    void clear();

    // 迭代器
    using iterator_v = typename std::vector<Type>::iterator;
    using const_iterator_v = typename std::vector<Type>::const_iterator;
    iterator_v begin();
    const_iterator_v begin() const;
    iterator_v end();
    const_iterator_v end() const;
};


// 构造函数
template <typename Type>
SqList<Type>::SqList(size_t n) : _data_v(n) {
    _capacity = n;
}

// 预留函数
template <typename Type>
void SqList<Type>::reserve(size_t n){
    return _data_v.reserve(n);
}

// 元素操作
template <typename Type>
void SqList<Type>::push_back(const Type &value){
    _data_v.push_back(value);
}

template <typename Type>
void SqList<Type>::push_back(Type &&value){
    _data_v.push_back(std::move(value));
}

template <typename Type>
void SqList<Type>::pop_back(){
    if(!_data_v.empty()){
        _data_v.pop_back();
    }
}

template <typename Type>
void SqList<Type>::insert(size_t index, const Type &value){
    if(index > _data_v.size()){
        throw std::out_of_range("下标越界");
    }
    _data_v.insert(_data_v.begin() + index, value);
}

template <typename Type>
void SqList<Type>::insert(const_iterator iter, const Type &value){
    if(iter < _data_v.begin() || iter > _data_v.end()){
        throw std::out_of_range("迭代器越界");
    }
    _data_v.insert(iter, value);
}

// 元素访问
template <typename Type>
Type &SqList<Type>::front(){
    return _data_v.front();
}

template <typename Type>
const Type &SqList<Type>::front() const{
    return _data_v.front();
}

template <typename Type>
Type &SqList<Type>::back(){
    return _data_v.back();
}

template <typename Type>
const Type &SqList<Type>::back() const{
    return _data_v.back();
}

template <typename Type>
Type &SqList<Type>::operator[](size_t index){
    return _data_v[index];
}

template <typename Type>
const Type &SqList<Type>::operator[](size_t index) const{
    return _data_v[index];
}

// 容量
template <typename Type>
size_t SqList<Type>::size() const{
    return _data_v.size();
}

template <typename Type>
size_t SqList<Type>::capacity() const{
    return -_data_v.capacity();
}

template <typename Type>
void SqList<Type>::resize(size_t newSize){
    size_t old_capacity = _data_v.capacity();
    _data_v.resize(old_capacity * 2); // 扩容为原来的两倍
}


template <typename Type>
bool SqList<Type>::empty() const{
    return _data_v.empty();
}

template <typename Type>
void SqList<Type>::clear(){
    _data_v.clear();
}

// 迭代器
template <typename Type>
typename SqList<Type>::iterator_v SqList<Type>::begin(){
    return _data_v.begin();
}

template <typename Type>
typename SqList<Type>::const_iterator_v SqList<Type>::begin() const{
    return _data_v.begin();
}

template <typename Type>
typename SqList<Type>::iterator_v SqList<Type>::end(){
    return _data_v.end();
}

template <typename Type>
typename SqList<Type>::const_iterator_v SqList<Type>::end() const{
    return _data_v.end();
}

#endif // SQLIST_H