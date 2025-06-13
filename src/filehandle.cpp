#include "filehandle.h"
#include <iostream>
#include <fstream>
#include <iomanip>

/**
 * @brief 从文件中读取文本，提取单词并存储到 _word_set 中
 *
 * 打开指定的文件，逐个读取单词，对每个单词进行处理（转小写、去除非字母字符），
 * 然后将非空单词添加到 _word_set 集合中
 *
 * @return 如果文件成功打开并处理返回true，否则返回false
 */
bool FileHandle::read_from_file(){
    std::ifstream file(_filename);
    if (!file.is_open()){
        std::cerr << "无法打开文件: " << _filename << std::endl;
        return false;
    }
    std::string word;
    while (file >> word){
        process_word(word);
        if (!word.empty()){
            _word_set.push_back(word);
        }
    }
    file.close();
    return true;
}

/**
 * @brief 获取处理后的单词集合
 *
 * 提供对内部存储的单词集合的只读访问，允许外部代码遍历或查询单词，
 * 但不允许修改集合内容。
 *
 * @return 返回单词集合的常量引用
 */
const SqList<std::string>& FileHandle::get_word_set() const {
        return _word_set;
}
/**
 * @brief 处理单个单词，转换为小写并移除非字母字符
 *
 * 该函数将单词中的所有字母转换为小写，并移除单词开头和结尾的非字母字符。
 * 如果单词全部由非字母字符组成，处理后将变为空字符串。
 *
 * @param word 需要处理的单词，作为引用传递，直接修改原字符串
 */
void FileHandle::process_word(std::string &word)
{
    // 转换为小写
    for (size_t i = 0; i < word.length(); i++){
        word[i] = std::tolower(static_cast<unsigned char>(word[i]));
    }
    // 查找第一个字母字符
    size_t start = 0;
    while (start < word.length() && !isalpha(word[start])){
        start++;
    }
    // 查找最后一个字母字符
    size_t end = word.length();
    while (end > start && !isalpha(word[end - 1])){
        end--;
    }
    // 修改原字符串，只保留字母字符部分
    if (start > 0 || end < word.length()){
        word = word.substr(start, end - start);
    }
}
/**
 * @brief 将词频统计结果写入指定的输出文件
 * @param frequency_table 包含单词及其出现频率的表格
 * @param outfile 输出文件的路径
 * @return 如果写入成功返回true，否则返回false
 */
bool FileHandle::write_into_file(const FreqTable& frequency_table,
                        const std::string& outfile)
{
    std::ofstream out(outfile);
    if (!out.is_open()) {
        std::cerr << "无法打开输出文件: " << outfile << std::endl;
        return false;
    }
    out << std::left <<std::setw(15)<<"单词" << ": "<<std::left << std::setw(15)<< "频率" << std::endl;
    for (size_t i = 0; i < frequency_table.size(); i++) {
        out << std::left <<std::setw(15)<<frequency_table[i].first << ": " <<std::left <<std::setw(15)<< frequency_table[i].second << std::endl;
    }
    out.close();
    return true;
}
