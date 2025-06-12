#ifndef BASE_SQLIST_H
#define BASE_SQLIST_H

#include "basewordcounter.h"

using string=std::string;

class BaseSqList: public BaseWordCounter{
protected:
    FreqTable _data;
public:
    BaseSqList()=default;
    // 删除构造函数实现
    // BaseSqList(const SqList<string> &baseTable);
    virtual void load(const SqList<string> &baseTable) override;
    virtual const FreqTable &get_frequency_table() const override;
protected:
    // 返回 [下标,查找次数]
    virtual Pair<size_t,size_t> find_word(const string &word) = 0;
private:
    virtual Ret _search_word(const string &word) override;
};


#endif // BASE_SQLIST_H