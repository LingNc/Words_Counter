#include "chainhash.hpp"
#include "basetype.h"
#include "sqlist.hpp"
#include "pair.hpp"
#include "sqlink.hpp"
#include "sharedptr.hpp"

ChainHash::ChainHash(size_t size):_elementCount(0){
    // 初始化哈希表中的每个链表
    for (size_t i = 0; i < size; i++) {
        _hash.push_back(SqLink<std::string>());
    }
};

ChainHash::~ChainHash(){};

// 名字
std::string ChainHash::name() const {
    return "链式哈希表";
}

uint32_t ChainHash::fnv1a_hash(const std::string& str) {
    const uint32_t FNV_prime = 16777619u;
    const uint32_t offset_basis = 2166136261u;

    uint32_t hash = offset_basis;

    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= FNV_prime;
    }

    return hash;
}

// 载入基础单词表
void ChainHash::load(const SqList<std::string> &baseTable) {
    for (size_t i = 0; i < baseTable.size(); i++){
        insert_word(baseTable[i]);
    }
}

void ChainHash::resize(size_t newsize){
    if (newsize == 0){
        newsize = _hash.size() * 2;
    }

    HashTable newhash(newsize);

    for (size_t i = 0; i < _hash.size(); i++){
        SqLink<std::string> &cur = _hash[i];
        if (cur.get_head()){
            utils::shared_ptr<Node<std::string>> tmp = cur.get_head();
            while (tmp){
                std::string word = tmp->data;
                size_t freq = tmp->frequency;

                uint32_t hashvalue = fnv1a_hash(word) % newsize;
                utils::shared_ptr<Node<std::string>> new_node = newhash[hashvalue].push_back(word);
                new_node->frequency = freq;
                tmp = tmp->next;
            }

        }
    }
    _hash = newhash;

}
// 插入或更新一个单词的频率
void ChainHash::insert_word(const std::string& word) {
    if ((double)_elementCount / _hash.size() > 0.7){
        resize();
    }

    uint32_t hashvalue = fnv1a_hash(word) % _hash.size();
    if (_hash[hashvalue].add(word)){
        _elementCount++;
    };
}

// 获取频率表
const FreqTable& ChainHash::get_frequency_table() const {
    FreqTable *freqtable = new FreqTable();

    for (size_t i = 0; i < _hash.size(); i++){
        utils::shared_ptr<Node<std::string>> current = _hash[i].get_head();

        while (current) {
            WordItem item(current->data, current->frequency);
            freqtable->push_back(item);
            current = current->next;
        }
    }

    return *freqtable;
}

// 查找单词词频，返回是否(成功,词频)和比较次数
ChainHash::Ret ChainHash::_search_word(const std::string &word) {
    uint32_t hashvalue = fnv1a_hash(word) % _hash.size();

    Ret result = {false, 0, 0, 1};
    Pair<utils::shared_ptr<Node<std::string>>, size_t> node = _hash[hashvalue].find(word);

    if (node.first) {
        result.isFound = true;
        result.wordFreq = node.first->frequency;
        result.comparisons += node.second;
    }

    return result;
}