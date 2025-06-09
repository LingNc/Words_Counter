#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include "basetype.h"

/**
 * @class FileHandle
 * @brief 处理文件中的单词，提取并存储单词集合
 * 
 * 该类负责从指定文件中读取文本，提取单词，
 * 并对单词进行预处理（如转换为小写、去除非字母字符），
 * 最终将处理后的单词存储在一个集合中
 */
class FileHandle{
private:
    const std::string _filename;  // 要处理的文件名
    SqList<std::string> _word_set;  // 存储处理后的单词集合

public:    
    FileHandle(const std::string& filename) : _filename(filename){};
    ~FileHandle(){};    
    
    /**
     * @brief 从文件中获取所有单词并进行处理
     * @return 如果文件成功打开并处理返回true，否则返回false
     */
    bool read_from_file();

    /**
     * @brief 获取处理后的单词集合
     * @return 返回单词集合的常量引用，允许外部访问但不允许修改
     */
    const SqList<std::string>& get_word_set() const;

    /**
     * @brief 将词频统计结果写入指定的输出文件
     * @param frequency_table 包含单词及其出现频率的表格
     * @param outfile 输出文件的路径
     * @return 如果写入成功返回true，否则返回false
     */
    bool write_into_file(const FreqTable& frequency_table, 
                        const std::string& outfile);
private:
    /**
     * @brief 处理单个单词（转小写并移除非字母字符）
     * @param word 需要处理的单词，作为引用传递，直接修改原字符串
     */
    void process_word(std::string &word);
};

#endif