/**
 * @file linkedlistwordcounter.h
 * @author Pete
 * @brief 用链表实现的词频统计器，继承自 BaseWordCounter。
 * @date 2025-06-13
 */

#ifndef LINKEDLIST_WORD_COUNTER_H
#define LINKEDLIST_WORD_COUNTER_H

#include "basewordcounter.h"
#include "linkedlist.hpp"
#include <string>


/**
 * @brief 用链表实现的词频统计器
 * 继承自 BaseWordCounter，底层用 LinkedList 存储单词及其词频。
 */
class LinkedListWordCounter : public BaseWordCounter {
public:
    /**
     * @brief 构造函数，初始化链表词频统计器
     */
    LinkedListWordCounter() = default;
    /**
     * @brief 析构函数
     */
    ~LinkedListWordCounter() override = default;
    /**
     * @brief 获取统计器名称
     * @return 名称字符串
     */
    std::string name() const override {
        return "链表直接查找";
    }

    /**
     * @brief 加载基础单词表，清空当前统计表并批量插入单词
     * @param baseTable 外部传入的单词顺序表
     */
    void load(const SqList<std::string> &baseTable) override {
        freq_table.clear(); // 清空链表
        for (int i = 0; i < baseTable.size(); i++){
            insert_word(baseTable[i]);
        }
    }

    /**
     * @brief 插入或更新一个单词的频率
     * @param word 单词
     */
    void insert_word(const std::string& word) override {
        for (auto node = freq_table.get_head(); node; node = node->next) {
            if (node->data.first == word) {
                ++node->data.second;
                return;
            }
        }
        freq_table.push_back({word, 1});
    }

    /**
     * @brief 获取词频表，转换为 FreqTable 类型
     * @return FreqTable 引用
     */
    const FreqTable& get_frequency_table() const override {
        static FreqTable table;
        table.clear();
        for (auto node = freq_table.get_head(); node; node = node->next) {
            table.push_back(WordItem(node->data.first, node->data.second));
        }
        return table;
    }

protected:
    /**
     * @brief 查找单词，返回查找结果结构体
     * @param word 单词
     * @return Ret 结构体，包含是否找到、词频、比较次数等
     */
    Ret _search_word(const std::string &word) override {
        Ret ret;
        ret.isFound = false;
        ret.wordFreq = 0;
        ret.comparisons = 0;
        for (auto node = freq_table.get_head(); node; node = node->next) {
            ++ret.comparisons;
            if (node->data.first == word) {
                ret.isFound = true;
                ret.wordFreq = node->data.second;
                break; // 找到后立即退出循环
            }
        }
        return ret;
    }

private:
    /**
     * @brief 链表，存储<单词, 词频>对
     */
    LinkedList<std::pair<std::string, size_t>> freq_table;
};

#endif // LINKEDLIST_WORD_COUNTER_H