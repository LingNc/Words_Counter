#ifndef BASE_WORD_COUNTER_H
#define BASE_WORD_COUNTER_H

#include "basetype.h"

class BaseWordCounter {
public:
    virtual ~BaseWordCounter() = default;

    // 插入或更新一个单词的频率
    virtual void insert_word(const std::string& word) = 0;

    // 查找一个单词的频率，返回是否找到，并输出比较次数
    virtual bool search_word(const std::string& word, int& frequency, int& comparisons) = 0;

    // 获取频率表
    virtual FreqTable& get_frequency_table() const = 0;

    // 获取ASL（查找成功的平均比较次数）
    virtual double get_ASL() const = 0;

private:
    // 获取插入过程中总比较次数（用于ASL计算）
    virtual int get_total_comparisons() const = 0;

    // 获取成功查找的次数（用于ASL计算）
    virtual int get_success_count() const = 0;
};

#endif // BASE_WORD_COUNTER_H