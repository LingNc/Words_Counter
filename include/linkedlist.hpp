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
#include <optional>
#include <utility> // for std::pair

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
 * @brief 单链表类，支持尾插、清空、遍历、范围for等操作
 * @tparam Type 链表存储的数据类型
 */
template <typename Type>
class LinkedList {
    ListNode<Type>* head; ///< 链表头指针
public:
    // 禁用拷贝构造和赋值，防止浅拷贝导致内存问题
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    /**
     * @brief 构造函数，初始化空链表
     */
    LinkedList() : head(nullptr) {}
    /**
     * @brief 析构函数，释放链表所有节点
     */
    ~LinkedList();

    /**
     * @brief 尾插法添加元素
     * @param value 要插入的数据
     */
    void push_back(const Type& value);

    /**
     * @brief 清空链表，释放所有节点
     */
    void clear() {
        while (head) {
            ListNode<Type>* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    /**
     * @brief 获取头指针，便于外部遍历
     * @return 链表头节点指针
     */
    ListNode<Type>* get_head() const { return head; }

    // ================== 迭代器支持范围for ==================
    /**
     * @brief 非const迭代器，支持范围for和标准库算法
     */
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = Type*;
        using reference = Type&;

        iterator(ListNode<Type>* n) : node(n) {}
        iterator& operator++() { if (node) node = node->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }
        bool operator==(const iterator& other) const { return node == other.node; }
        bool operator!=(const iterator& other) const { return node != other.node; }
    private:
        ListNode<Type>* node;
    };

    /**
     * @brief const迭代器，支持const对象的范围for和标准库算法
     */
    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = const Type*;
        using reference = const Type&;

        const_iterator(const ListNode<Type>* n) : node(n) {}
        const_iterator& operator++() { if (node) node = node->next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        reference operator*() const { return node->data; }
        pointer operator->() const { return &node->data; }
        bool operator==(const const_iterator& other) const { return node == other.node; }
        bool operator!=(const const_iterator& other) const { return node != other.node; }
    private:
        const ListNode<Type>* node;
    };

    /**
     * @brief 获取非const迭代器起始位置
     */
    iterator begin() { return iterator(head); }
    /**
     * @brief 获取非const迭代器结束位置
     */
    iterator end() { return iterator(nullptr); }
    /**
     * @brief 获取const迭代器起始位置
     */
    const_iterator begin() const { return const_iterator(head); }
    /**
     * @brief 获取const迭代器结束位置
     */
    const_iterator end() const { return const_iterator(nullptr); }
};

// ================== 成员函数实现 ==================

/**
 * @brief 析构函数，释放链表所有节点
 */
template <typename Type>
LinkedList<Type>::~LinkedList() {
    ListNode<Type>* cur = head;
    while (cur) {
        ListNode<Type>* tmp = cur;
        cur = cur->next;
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

#endif // LINKEDLIST_HPP