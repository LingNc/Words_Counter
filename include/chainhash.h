#ifndef CHAININFHASH_HPP
#define CHAININFHASH_HPP

#include "basewordcounter.h"
#include "sqlink.hpp"
#include <cstdint>

class ChainHash : public BaseWordCounter{
using HashTable = SqList<SqLink<std::string>>;
private:
    HashTable _hash;
    size_t _elementCount;
    FreqTable _freqtable;
    uint32_t fnv1a_hash(const std::string& str);
    void resize(size_t size = 0);   //默认扩容两倍
public:
    ChainHash(size_t size = 5000);//默认5000
    virtual ~ChainHash() override;

    // 载入基础单词表
    virtual void load(const SqList<std::string> &baseTable) override;

    // 插入或更新一个单词的频率
    virtual void insert_word(const std::string& word) override;

    // 获取频率表
    virtual const FreqTable &get_frequency_table() override;

    // 名字
    virtual std::string name() const override;
protected:
    // 查找单词词频，返回是否(成功,词频)和比较次数
    virtual Ret _search_word(const std::string &word) override;
};

#endif