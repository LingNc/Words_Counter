#include "basewordcounter.h"
#include <stdexcept>
#include <chrono>

void BaseWordCounter::load(const SqList<std::string> &baseTable){
    for (const auto &item : baseTable) {
        insert_word(item);
    }
}

double BaseWordCounter::get_ASL() const {
    if (_successCounts == 0) {
        // throw std::runtime_error("ASL 计算除零错误");
        return -1.0;
    }
    return static_cast<double>(_totalCompares)/_successCounts;
}

BaseWordCounter::Ret BaseWordCounter::search_word(const std::string &word){
    // 计算时间
    auto start=std::chrono::high_resolution_clock::now();
    auto result=_search_word(word);
    auto end=std::chrono::high_resolution_clock::now();
    result.time_us=std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    // 更新成功比较次数和成功次数
    if(result.isFound) {
        _totalCompares += result.comparisons;
        _successCounts++;
    }
    return result;
}

// 获取基类名字
std::string BaseWordCounter::name() const {
    return "基类Counter";
}