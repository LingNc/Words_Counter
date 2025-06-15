#include "binarysqlist.h"

// 初始化
BinarySqlist::BinarySqlist(const SqList<string> &baseTable){
    load(baseTable);
}

// 名字
string BinarySqlist::name() const{
    return "二分查找顺序表";
}

// 返回第一个大于等于的地方
Pair<size_t,size_t> BinarySqlist::find_word(const string &word){
    int64_t left=-1,right=_data.size();
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

    // 返回插入位置
    return { right,comparisons };
}

void BinarySqlist::insert_word(const std::string &word){
    auto [index,comparisons]=find_word(word);
    // 如果存在更新频率
    if(index<_data.size()&&_data[index].first==word)
        _data[index].second++;
    // 如果不存在，插入新单词
    else
        _data.insert(index,{ word,1 });
}

BaseWordCounter::Ret BinarySqlist::_search_word(const string &word){
    Ret ret;
    ret.isFound=false;
    auto [index,times]=find_word(word);
    ret.comparisons=times;
    // 如果找到
    // 修复二分的时候如果大于没有判断等于的问题
    if(index!=_data.size()){
        if(_data[index].first==word){
            // 在二分的情况下，这里也算一次比较
            ret.comparisons++;
            ret.isFound=true;
            ret.wordFreq=_data[index].second;
        }
    }
    return ret;
}