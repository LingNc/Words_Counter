#include "sequensqlist.h"

// 找到的地方，如果没有找到返回size()，和查找次数
Pair<size_t,size_t> SequenSqList::find_word(const string &word){
    size_t index=_data.size();
    size_t comparisons=0;
    for(size_t i=0;i<_data.size();i++){
        comparisons++;
        if(_data[i].first==word){
            index=i;
            break;
        }
    }
    return { index,comparisons };
}

void SequenSqList::insert_word(const std::string &word){
    auto [index,comparisons]=find_word(word);
    // 存在更新
    if(index<_data.size()&&_data[index].first==word)
        _data[index].second++;
    // 不存在添加
    else
        _data.push_back({word, 1});
}