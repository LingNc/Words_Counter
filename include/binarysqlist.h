#include "basesqlist.h"

class BinarySqlist: public BaseSqList{
public:
    BinarySqlist()=default;
    BinarySqlist(const SqList<string> &baseTable);
    virtual void insert_word(const std::string &word) override;
    virtual string name() const override;
private:
    virtual Pair<size_t,size_t> find_word(const string &word) override;
    virtual Ret _search_word(const string &word) override;
};