#include "basesqlist.h"
#include <algorithm>

BaseSqList::BaseSqList(const SqList<string> &baseTable){
    load(baseTable);
}

void BaseSqList::load(const SqList<string> &baseTable){
    _data.clear();
    for(const auto &word : baseTable){
        insert_word(word);
    }
}

const FreqTable &BaseSqList::get_frequency_table() const{
    return _data;
}

BaseSqList::Ret BaseSqList::_search_word(const string &word){
    Ret ret;
    ret.isFound=false;
    auto [index,times]=find_word(word);
    ret.comparisons=times;
    // 如果找到
    if(index!=_data.size()){
        ret.isFound=true;
        ret.wordFreq = _data[index].second;
    }
    return ret;
}