#include <string>

namespace utils{
    // 计算字符串的显示宽度（处理中文字符）
    /*
        * @brief 计算字符串的显示宽度，处理中文字符
        * @param str 输入字符串
        * @return 返回字符串的显示宽度
        * @note 该函数用于计算字符串在终端或文本编辑器中的显示宽度，
        *       中文字符通常占用2个ASCII字符的宽度。
    */
    size_t display_width(const std::string &str);

    // 格式化输出，确保中文对齐
    /*
        * @brief 格式化字符串，使其在指定宽度内对齐
        * @param str 输入字符串
        * @param target_width 目标宽度
        * @return 返回格式化后的字符串
        * @note 如果字符串实际宽度小于目标宽度，则在后面填充空格。
        *       如果实际宽度大于或等于目标宽度，则返回原字符串。
        *       该函数专门处理中英文混合的对齐问题。
    */
    std::string format_aligned(const std::string &str,size_t target_width);
}