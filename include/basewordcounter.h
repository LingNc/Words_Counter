#ifndef BASE_WORD_COUNTER_H
#define BASE_WORD_COUNTER_H

#include "basetype.h"

class BaseWordCounter{
public:
    struct Ret{
        // 查找结果
        bool isFound;
        // 查找时间，单位微秒
        int64_t time_us;
        // 查找词频
        size_t wordFreq;
        // 本次比较次数
        size_t comparisons;
    };
    virtual ~BaseWordCounter()=default;

    // 载入基础单词表
    virtual void load(const SqList<std::string> &baseTable);

    // 插入或更新一个单词的频率
    virtual void insert_word(const std::string& word) = 0;

    // 获取频率表
    virtual const FreqTable& get_frequency_table() const = 0;

    // 获取特定类的称号
    virtual std::string name() const;

    // 查找一个单词的频率，返回花费时间和词频
    Ret search_word(const std::string& word);

    // 获取ASL（查找成功的平均比较次数）
    double get_ASL() const;
protected:
    // 查找单词词频，返回是否(成功,词频)
    virtual Ret _search_word(const std::string &word) = 0;
private:
    size_t _totalCompares=0;
    size_t _successCounts=0;
};

#endif // BASE_WORD_COUNTER_H