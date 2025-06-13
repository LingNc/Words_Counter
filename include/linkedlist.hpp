// linkedlist.hpp
/**
 * @file linkedlist.hpp
 * @author Pete
 * @brief 单链表实现，支持尾插、清空、下标访问等操作。
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
    Type data;                ///< 节点数据
    ListNode* next;           ///< 指向下一个节点的指针
    ListNode(const Type& d) : data(d), next(nullptr) {}
};

/**
 * @brief 单链表类，支持尾插、清空、遍历、下标访问等操作
 * @tparam Type 链表存储的数据类型
 */
template <typename Type>
class LinkedList {
    ListNode<Type>* head; ///< 链表头指针
public:
    /**
     * @brief 构造函数，初始化空链表
     */
    LinkedList();
    /**
     * @brief 析构函数，释放链表所有节点
     */
    ~LinkedList();
    /**
     * @brief 下标访问，返回第index个元素的常量引用
     * @param index 下标
     * @return 第index个元素的常量引用
     * @throws std::out_of_range 如果下标越界
     */
    const Type& operator[](int index) const;
    /**
     * @brief 尾插添加元素
     * @param value 要插入的数据
     */
    void push_back(const Type& value);
    /**
     * @brief 清空链表，释放所有节点
     */
    void clear();
    /**
     * @brief 获取头指针，便于外部遍历
     * @return 链表头节点指针
     */
    ListNode<Type>* get_head() const;
};

// ================== 成员函数实现 ==================

/**
 * @brief 构造函数，初始化空链表
 */
template <typename Type>
LinkedList<Type>::LinkedList() : head(nullptr) {}

/**
 * @brief 析构函数，释放链表所有节点
 */
template <typename Type>
LinkedList<Type>::~LinkedList() {
    clear();
}

/**
 * @brief 清空链表，释放所有节点
 */
template <typename Type>
void LinkedList<Type>::clear() {
    while (head) {
        ListNode<Type>* tmp = head;
        head = head->next;
        delete tmp;
    }
}

/**
 * @brief 尾插法添加元素
 * @param value 要插入的数据
 */
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

/**
 * @brief 获取头指针，便于外部遍历
 * @return 链表头节点指针
 */
template <typename Type>
ListNode<Type>* LinkedList<Type>::get_head() const { return head; }

/**
 * @brief 下标访问，返回第index个元素的常量引用
 * @param index 下标
 * @return 第index个元素的常量引用
 * @throws std::out_of_range 如果下标越界
 */
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