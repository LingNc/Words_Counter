#ifndef TOOLS_H
#define TOOLS_H

namespace utils{
    // 拷贝工具
    template<typename Input,typename Output>
    void copy(Input first,Input last,Output result){
        while(first!=last){
            *result++=*first++;
        }
    }
}

#endif // TOOLS_H