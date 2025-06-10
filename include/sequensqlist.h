#include "basesqlist.h"

class SequenSqList: protected BaseSqList{
public:
    SequenSqList()=default;
    SequenSqList(const SqList<string> &baseTable):BaseSqList(baseTable){};
    virtual void insert_word(const std::string &word) override;
private:
    virtual Pair<size_t, size_t> find_word(const string &word) override;
};