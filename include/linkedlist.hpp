#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <string>
#include <optional>
#include "basewordcounter.h"
#include <string>
#include <list>
#include <utility> // for std::pair


// 单链表节点定义
template <typename Type>
struct ListNode {
    Type data;
    ListNode* next;
    ListNode(const Type& d) : data(d), next(nullptr) {}
};

// 单链表类定义
template <typename Type>
class LinkedList {
    ListNode<Type>* head;
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList();

    void push_back(const Type& value);
    int find_word(const Type& word) const;
};

// 析构函数，释放链表内存
template <typename Type>
LinkedList<Type>::~LinkedList() {
    ListNode<Type>* cur = head;
    while (cur) {
        ListNode<Type>* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

// 尾插法添加元素
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

// 查找单词，返回首次出现的下标，否则返回-1
template <typename Type>
int LinkedList<Type>::find_word(const Type& word) const {
    ListNode<Type>* cur = head;
    int idx = 0;
    while (cur) {
        if (cur->data == word) return idx;
        cur = cur->next;
        ++idx;
    }
    return -1;
}


#endif // LINKEDLIST_HPP