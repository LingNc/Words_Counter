#ifndef LINKEDLIST_WORD_COUNTER_H
#define LINKEDLIST_WORD_COUNTER_H

#include "basewordcounter.h"
#include "linkedlist.hpp"
#include <chrono>
#include <string>
#include <utility>

// 用链表实现的词频统计器
class LinkedListWordCounter : public BaseWordCounter {
public:
    LinkedListWordCounter() = default;
    ~LinkedListWordCounter() override = default;

    void load(const SqList<std::string> &baseTable) override {
        freq_table = LinkedList<std::pair<std::string, size_t>>();
    }

    void insert_word(const std::string& word) override {
        size_t idx = freq_table.find_word(std::make_pair(word, 0));
        size_t curIdx = 0;
        for (auto node = freq_table.get_head(); node; node = node->next, ++curIdx) {
            if (node->data.first == word) {
                ++node->data.second;
                return;
            }
        }
        freq_table.push_back({word, 1});
    }

    // 返回FreqTable类型，转换链表内容
    const FreqTable& get_frequency_table() const override {
        static FreqTable table;
        table.clear();
        for (auto node = freq_table.get_head(); node; node = node->next) {
            table.push_back(WordItem(node->data.first, node->data.second));
        }
        return table;
    }

protected:
    Ret _search_word(const std::string &word) override {
        Ret ret{};
        size_t compares = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (auto node = freq_table.get_head(); node; node = node->next) {
            ++compares;
            if (node->data.first == word) {
                auto end = std::chrono::high_resolution_clock::now();
                ret.isFound = true;
                ret.wordFreq = node->data.second;
                ret.comparisons = compares;
                ret.time_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                return ret;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        ret.isFound = false;
        ret.wordFreq = 0;
        ret.comparisons = compares;
        ret.time_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        return ret;
    }

private:
    LinkedList<std::pair<std::string, size_t>> freq_table;
};

#endif // LINKEDLIST_WORD_COUNTER_H