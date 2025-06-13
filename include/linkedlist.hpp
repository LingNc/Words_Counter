// linkedlist.hpp
/**
 * @file linkedlist.hpp
 * @author Pete
 * @brief 单链表及其迭代器实现，支持范围for、尾插、清空等操作。
 * @date 2025-06-13
 */
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <string>

/**
 * @brief 单链表节点定义
 * @tparam Type 节点存储的数据类型
 */
template <typename Type>
struct ListNode {
    Type data;
    ListNode* next;
    ListNode(const Type& d) : data(d), next(nullptr) {}
};

/**
 * @brief 单链表类，支持尾插、清空、遍历、范围for等操作
 * @tparam Type 链表存储的数据类型
 */
template <typename Type>
class LinkedList {
    ListNode<Type>* head;
public:
    LinkedList();
    ~LinkedList();
    const Type& operator[](int index) const;
    void push_back(const Type& value);
    void clear();
    ListNode<Type>* get_head() const;
};

// ================== 成员函数实现 ==================

template <typename Type>
LinkedList<Type>::LinkedList() : head(nullptr) {}

template <typename Type>
LinkedList<Type>::~LinkedList() {
    clear();
}

template <typename Type>
void LinkedList<Type>::clear() {
    while (head) {
        ListNode<Type>* tmp = head;
        head = head->next;
        delete tmp;
    }
}

template <typename Type>
void LinkedList<Type>::push_back(const Type& value) {
    ListNode<Type>* node = new ListNode<Type>(value);
    if (!head) {
        head = node;
        return;
    }
    ListNode<Type>* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = node;
}

template <typename Type>
ListNode<Type>* LinkedList<Type>::get_head() const { return head; }

template <typename Type>
const Type& LinkedList<Type>::operator[](int index) const {
    ListNode<Type>* cur = head;
    int i = 0;
    while (cur && i < index) {
        cur = cur->next;
        ++i;
    }
    if (!cur) {
        throw std::out_of_range("LinkedList index out of range");
    }
    return cur->data;
}

#endif // LINKEDLIST_HPP