#include "binarysqlist.h"

// 初始化
BinarySqlist::BinarySqlist(const SqList<string> &baseTable){
    load(baseTable);
}

// 返回第一个大于等于的地方
Pair<size_t,size_t> BinarySqlist::find_word(const string &word){
    int64_t left = -1, right = _data.size();
    // 比较次数
    size_t comparisons=0;

    while(left+1<right){
        int64_t mid=left+((right-left)>>1);
        comparisons++;
        if(_data[mid].first>=word)
            right=mid;
        else
            left=mid;
    }
    // 如果找到
    if(right!=(int64_t)_data.size()){
        if(_data[right].first==word){
            return { right,comparisons };
        }
        else return { right,comparisons };
    }
    else return { right, comparisons };
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