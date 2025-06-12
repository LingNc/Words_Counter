#ifndef SQLIST_H
#define SQLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <bits/move.h>
#include <cstring>
#include <new>
#include "utils.hpp"

#define DEFAULT_CAPACITY 10

template <typename Type>
class SqList{
public:
    using iterator=Type *;
    using const_iterator=const Type *;
private:
    Type* _data;        // 数据数组指针
    size_t _capacity;  // 容量
    size_t _size;     // 实际大小

    // 扩容函数
    void _expand();
public:
    // 基本函数
    SqList();
    // 构造函数
    explicit SqList(size_t n);
    ~SqList();

    // 拷贝构造
    SqList(const SqList &other); // 禁止拷贝构造
    // 移动构造
    SqList(SqList &&other) noexcept;
    // 初始化列表构造函数
    SqList(std::initializer_list<Type> initList);

    // 拷贝赋值
    SqList &operator=(const SqList &other);
    // 移动赋值
    SqList &operator=(SqList &&other) noexcept;
    // 列表赋值
    SqList &operator=(std::initializer_list<Type> initList);

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
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
};


// 扩容函数
template <typename Type>
void SqList<Type>::_expand(){
    size_t new_capacity = _capacity * 2;
    Type* new_data = new Type[new_capacity];

    // 使用工具函数utils::move移动已有元素
    utils::move(_data, _data + _size, new_data);

    delete[] _data;
    _data = new_data;
    _capacity = new_capacity;
}

// 默认构造函数
template <typename Type>
SqList<Type>::SqList(): _capacity(DEFAULT_CAPACITY), _size(0) {
    _data = new Type[_capacity];
}

// 构造函数
template <typename Type>
SqList<Type>::SqList(size_t n): _size(0) {
    _capacity = (n > 0) ? n : DEFAULT_CAPACITY;
    _data=new Type[_capacity];

}

// 析构函数
template <typename Type>
SqList<Type>::~SqList(){
    if(_data) {  // 检查指针是否有效（防止移动后的对象析构出错）
        delete[] _data;
    }
}

// 拷贝构造
template <typename Type>
SqList<Type>::SqList(const SqList &other): _capacity(other._capacity), _size(other._size) {
    _data = new Type[_capacity];
    // 使用工具函数utils::copy复制元素
    utils::copy(other._data, other._data + other._size, _data);
}

// 移动构造
template <typename Type>
SqList<Type>::SqList(SqList &&other) noexcept
    : _data(other._data), _capacity(other._capacity), _size(other._size) {
    other._data = nullptr;
    other._capacity = 0;
    other._size = 0;
}

// 初始化列表构造
template <typename Type>
SqList<Type>::SqList(std::initializer_list<Type> initList)
    : _capacity((initList.size() > DEFAULT_CAPACITY) ? initList.size() : DEFAULT_CAPACITY), _size(initList.size()) {
    _data = new Type[_capacity];
    // 使用工具函数utils::copy
    utils::copy(initList.begin(), initList.end(), _data);
}

// 拷贝赋值
template <typename Type>
SqList<Type>& SqList<Type>::operator=(const SqList &other){
    // 自赋值检查
    if(this != &other){
        // 如果需要扩容
        if(other._size > _capacity) {
            delete[] _data;
            _capacity = other._capacity;
            _data = new Type[_capacity];
        }

        // 使用工具函数utils::copy复制元素
        utils::copy(other._data, other._data + other._size, _data);
        _size = other._size;
    }
    return *this;
}

// 移动赋值
template <typename Type>
SqList<Type>& SqList<Type>::operator=(SqList &&other) noexcept {
    if(this != &other){
        if(_data) {  // 检查当前指针是否有效
            delete[] _data;
        }
        _data = other._data;
        _capacity = other._capacity;
        _size = other._size;

        // 清空其他对象
        other._data = nullptr;
        other._capacity = 0;
        other._size = 0;
    }
    return *this;
}

// 初始化列表赋值运算符
template <typename Type>
SqList<Type>& SqList<Type>::operator=(std::initializer_list<Type> initList){
    clear();  // 清空当前内容

    // 如果需要扩容
    if(initList.size() > _capacity){
        delete[] _data;
        _capacity = initList.size();
        _data = new Type[_capacity];
    }
    utils::copy(initList.begin(), initList.end(), _data);
    _size=initList.size();
    return *this;
}

// 预留函数
template <typename Type>
void SqList<Type>::reserve(size_t n){
    if(n > _capacity){
        Type* new_data = new Type[n];

        // 使用工具函数utils::move移动已有元素
        utils::move(_data, _data + _size, new_data);

        delete[] _data;
        _data = new_data;
        _capacity = n;
    }
}

// 元素操作
template <typename Type>
void SqList<Type>::push_back(const Type &value){
    if(_size >= _capacity){
        _expand();
    }
    _data[_size] = value;
    ++_size;
}

template <typename Type>
void SqList<Type>::push_back(Type &&value){
    if(_size >= _capacity){
        _expand();
    }
    _data[_size] = utils::move(value);
    ++_size;
}

template <typename Type>
void SqList<Type>::pop_back(){
    if(_size > 0){
        --_size;
    }
}

template <typename Type>
void SqList<Type>::insert(size_t index, const Type &value){
    if(index > _size){
        throw std::out_of_range("下标越界");
    }

    if(_size >= _capacity){
        _expand();
    }

    // 使用工具函数copy向后移动元素
    if(index < _size) {
        // 从后往前复制，避免覆盖
        for(size_t i = _size; i > index; --i) {
            _data[i] = utils::move(_data[i-1]);
        }
    }

    _data[index] = value;
    ++_size;
}

template <typename Type>
void SqList<Type>::insert(const_iterator iter, const Type &value){
    if(iter < _data || iter > _data + _size){
        throw std::out_of_range("迭代器越界");
    }
    size_t index = iter - _data;
    insert(index, value);
}

// 元素访问
template <typename Type>
Type &SqList<Type>::front(){
    if(_size == 0){
        throw std::out_of_range("SqList为空");
    }
    return _data[0];
}

template <typename Type>
const Type &SqList<Type>::front() const{
    if(_size == 0){
        throw std::out_of_range("SqList为空");
    }
    return _data[0];
}

template <typename Type>
Type &SqList<Type>::back(){
    if(_size == 0){
        throw std::out_of_range("SqList为空");
    }
    return _data[_size - 1];
}

template <typename Type>
const Type &SqList<Type>::back() const{
    if(_size == 0){
        throw std::out_of_range("SqList为空");
    }
    return _data[_size - 1];
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
    return _size;
}

template <typename Type>
size_t SqList<Type>::capacity() const{
    return _capacity;
}

template <typename Type>
void SqList<Type>::resize(size_t newSize){
    if(newSize > _capacity){
        reserve(newSize);
    }
    _size = newSize;
}

template <typename Type>
bool SqList<Type>::empty() const{
    return _size == 0;
}

template <typename Type>
void SqList<Type>::clear(){
    _size = 0;
}

// 迭代器
template <typename Type>
typename SqList<Type>::iterator SqList<Type>::begin(){
    return _data;
}

template <typename Type>
typename SqList<Type>::const_iterator SqList<Type>::begin() const{
    return _data;
}

template <typename Type>
typename SqList<Type>::iterator SqList<Type>::end(){
    return _data + _size;
}

template <typename Type>
typename SqList<Type>::const_iterator SqList<Type>::end() const{
    return _data + _size;
}

// 取消引入
#undef DEFAULT_CAPACITY

#endif // SQLIST_H