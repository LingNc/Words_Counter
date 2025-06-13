#include "openhash.h"
#include "basetype.h"
#include "sqlist.hpp"
#include "pair.hpp"
#include "sharedptr.hpp"

OpenHash::OpenHash(size_t size):_hash(size){};
OpenHash::~OpenHash(){};

// 名字
std::string OpenHash::name() const {
    return "开放地址哈希表";
}

uint32_t OpenHash::fnv1a_hash(const std::string &str){
    const uint32_t FNV_prime = 16777619u;
    const uint32_t offset_basis = 2166136261u;

    uint32_t hash = offset_basis;

    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= FNV_prime;
    }

    return hash;
}

void OpenHash::resize(size_t newsize){
    if (newsize == 0) {
        newsize = _hash.size() * 2;
    }
    Hash newhash(newsize);

    for (size_t i = 0; i < newsize; i++) {
        newhash[i] = Pair<std::string, size_t>("", 0);
    }

    for (size_t i = 0; i < _hash.size(); i++){
        if (!_hash[i].first.empty()){
            // 重新计算哈希值
            uint32_t hashvalue = fnv1a_hash(_hash[i].first) % newsize;
            size_t j = hashvalue;

            while (!newhash[j].first.empty()) {
                j = (j + 1) % newsize;
            }

            newhash[j].first = _hash[i].first;
            newhash[j].second = _hash[i].second;
        }
    }

    _hash = newhash;
}
// 载入基础单词表
void OpenHash::load(const SqList<std::string> &baseTable) {
    for (size_t i = 0; i < baseTable.size(); i++){
        insert_word(baseTable[i]);
    }
}

// 插入或更新一个单词的频率
void OpenHash::insert_word(const std::string& word) {
    uint32_t hashvalue = fnv1a_hash(word) % _hash.size();
    size_t i = hashvalue;

    //线性探索
    while(!_hash[i].first.empty() && _hash[i].first != word){
        i =(i + 1) % _hash.size();
        if (i == hashvalue){
            resize();
            insert_word(word);
            return;
        }
    }
    if (_hash[i].first.empty()){
        _hash[i].first = word;
        _hash[i].second = 1;
    }else{
        _hash[i].second++;
    }
}

// 获取频率表
const FreqTable& OpenHash::get_frequency_table() const {
    utils::shared_ptr<FreqTable> freqtable = utils::make_shared<FreqTable>();
    for (size_t i = 0; i < _hash.size(); i++){
        if (!_hash[i].first.empty()){
            freqtable->push_back(_hash[i]);
        }
    }
    return *freqtable;
}

// 查找单词词频，返回是否(成功,词频)和比较次数
OpenHash::Ret OpenHash::_search_word(const std::string &word) {
    uint32_t hashvalue = fnv1a_hash(word) % _hash.size();
    size_t i = hashvalue;

    Ret result = {false, 0, 0, 1};
    while (!_hash[i].first.empty()){
        result.comparisons++;

        if (_hash[i].first == word){
            result.isFound = true;
            result.wordFreq = _hash[i].second;
            break;
        }

        //当不配时, i++;
        i = (i + 1) % _hash.size();
        if (i == hashvalue)
            break;
    }

    return result;
}