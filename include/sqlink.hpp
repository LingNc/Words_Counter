#ifndef SQLINK_H
#define SQLINK_H

#include "pair.hpp"
#include "sharedptr.hpp"

template <typename type>
struct Node{
    type data;
    size_t frequency;
    utils::shared_ptr<Node<type>> next;
    Node(const type& value):data(value), frequency(1),next(nullptr){};
    ~Node(){};
};

template <typename type>
class SqLink{
private:
    utils::shared_ptr<Node<type>> _head;
public:
    SqLink():_head(nullptr){};
    ~SqLink(){};

    bool add(const type& value);

    void clear();

    //return the pointer if the sqlink contains the given value
    Pair<utils::shared_ptr<Node<type>>, size_t> find(const type& value) const;

    utils::shared_ptr<Node<type>> push_back(const type& value);

    // 获取链表头节点
    utils::shared_ptr<Node<type>> get_head() const {
        return _head;
    }
};
template <typename type>
Pair<utils::shared_ptr<Node<type>>, size_t> SqLink<type>::find(const type& value) const{
    utils::shared_ptr<Node<type>> cur = _head;
    size_t compa = 0;
    while (cur){
        compa++;
        if (cur->data == value){
            return {cur, compa};
        }
        cur = cur->next;
    }
    return {nullptr, compa};
}
template <typename type>
utils::shared_ptr<Node<type>> SqLink<type>::push_back(const type& value){
    utils::shared_ptr<Node<type>> new_node = utils::make_shared<Node<type>>(value);
    if (!_head){
        _head = new_node;
        return new_node;
    }
    utils::shared_ptr<Node<type>> tmp = _head;
    while (tmp->next)
        tmp = tmp->next;
    tmp -> next = new_node;
    return new_node;
}
template <typename type>
bool SqLink<type>::add(const type& value){
    Pair<utils::shared_ptr<Node<type>>, size_t> new_node = find(value);
    if (new_node.first){
        new_node.first->frequency++;
        return false;//没有添加新节点
    }

    push_back(value);
    return true; //添加新节点
}
template <typename type>
void SqLink<type>::clear(){
    _head.reset(); // 智能指针会自动清理整个链表
}

#endif
