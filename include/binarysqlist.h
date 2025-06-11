#include "basesqlist.h"

class BinarySqlist: public BaseSqList{
public:
    BinarySqlist()=default;
    BinarySqlist(const SqList<string> &baseTable);
    virtual void insert_word(const std::string &word) override;
private:
    virtual Pair<size_t, size_t> find_word(const string &word) override;
};