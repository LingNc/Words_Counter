#include "filehandle.h"
#include <iostream>
#include <fstream>
#include "sqlist.hpp"

/**
 * @brief 从文件中读取文本，提取单词并存储到 _word_set 中
 * 
 * 打开指定的文件，逐个读取单词，对每个单词进行处理（转小写、去除非字母字符），
 * 然后将非空单词添加到 _word_set 集合中
 * 
 * @return 如果文件成功打开并处理返回true，否则返回false
 */
bool FileHandle::get_word_set(){
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

