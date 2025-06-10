#include "binarysqlist.h"

// 返回第一个大于等于的地方
Pair<size_t,size_t> BinarySqlist::find_word(const string &word){
    int64_t left = -1, right = _data.size();
    // 比较次数
    size_t comparisons=0;

    while(left+1<right){
        int64_t mid=left+(right-left)>>1;
        comparisons++;
        if(_data[mid].first>=word)
            right=mid;
        else
            left=mid;
    }
    // 如果没找到
    if(left+1==_data.size()||_data[left+1].first!=word){
        left=_data.size();
    }
    return { left,comparisons };
}

void BinarySqlist::insert_word(const std::string &word){
    auto [index,comparisons]=find_word(word);
    // 如果存在更新频率
    if(index<_data.size()&&_data[index].first==word)
        _data[index].second++;
    // 如果不存在，插入新单词
    else
        _data.insert(index, {word, 1});
}