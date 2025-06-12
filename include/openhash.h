#ifndef OPENHSAH_HPP
#define OPENHSAH_HPP

#include "basewordcounter.h"
#include <cstdint>

class OpenHash : public BaseWordCounter{
using Hash = SqList<Pair<std::string, size_t>>;
private:
    Hash _hash;
    uint32_t fnv1a_hash(const std::string& str);
    // 重新调整哈希表大小，默认扩大为原来的两倍
    void resize(size_t newsize = 0);
public:
    OpenHash(size_t size = 5000);
    virtual ~OpenHash() override;

    // 载入基础单词表
    virtual void load(const SqList<std::string> &baseTable) override;

    // 插入或更新一个单词的频率
    virtual void insert_word(const std::string& word) override;

    // 获取频率表
    virtual const FreqTable& get_frequency_table() const override;

    // 名字
    virtual std::string name() const override;
protected:
    // 查找单词词频，返回是否(成功,词频)和比较次数
    virtual Ret _search_word(const std::string &word) override;
};

#endif