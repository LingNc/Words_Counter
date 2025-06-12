#include "basesqlist.h"

class SequenSqList: public BaseSqList{
public:
    SequenSqList()=default;
    SequenSqList(const SqList<string> &baseTable);
    virtual void insert_word(const std::string &word) override;
    virtual string name() const override;
private:
    virtual Pair<size_t, size_t> find_word(const string &word) override;
};