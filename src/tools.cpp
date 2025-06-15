#include "tools.h"

namespace utils{
    size_t display_width(const std::string &str){
        size_t width=0;
        for(size_t i=0; i<str.length(); ){
            unsigned char c=str[i];
            if(c<=0x7F){
                // ASCII字符，宽度为1
                width+=1;
                i+=1;
            }
            else if((c&0xE0)==0xC0){
                // UTF-8 2字节字符，宽度为1
                width+=1;
                i+=2;
            }
            else if((c&0xF0)==0xE0){
                // UTF-8 3字节字符（包括中文），宽度为2
                width+=2;
                i+=3;
            }
            else if((c&0xF8)==0xF0){
                // UTF-8 4字节字符，宽度为2
                width+=2;
                i+=4;
            }
            else{
                // 其他情况，按1个字符处理
                width+=1;
                i+=1;
            }
        }
        return width;
    }

    std::string format_aligned(const std::string &str,size_t target_width){
        size_t actual_width=display_width(str);
        if(actual_width>=target_width){
            return str;
        }
        size_t padding=target_width-actual_width;
        return str+std::string(padding,' ');
    }
}