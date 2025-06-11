#ifndef LINKEDLIST_WORD_COUNTER_H
#define LINKEDLIST_WORD_COUNTER_H

#include "basewordcounter.h"
#include <chrono>
#include <string>

class LinkedListWordCounter : public BaseWordCounter {
public:
    LinkedListWordCounter() = default;
    ~LinkedListWordCounter() override = default;

    // 载入基础单词表
    void load(const SqList<std::string> &baseTable) override {
        freq_table.clear();
    }

    // 插入或更新一个单词的频率
    void insert_word(const std::string& word) override {
        for (auto& wf : freq_table) {
            if (wf.first == word) {
                ++wf.second;
                return;
            }
        }
        // std::cout << "插入新单词: " << word << std::endl;
        freq_table.push_back({word, 1});
    }

    // 直接用基类的FreqTable类型
    const FreqTable& get_frequency_table() const override {
        return freq_table;
    }

protected:
    // 查找单词词频，返回是否(成功,词频)
    Ret _search_word(const std::string &word) override {
        Ret ret{};
        size_t compares = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& wf : freq_table) {
            ++compares;
            if (wf.first == word) {
                auto end = std::chrono::high_resolution_clock::now();
                ret.isFound = true;
                ret.wordFreq = wf.second;
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
    FreqTable freq_table;
};

#endif // LINKEDLIST_WORD_COUNTER_H