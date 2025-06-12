#ifndef TOOLS_H
#define TOOLS_H

#include <utility>

namespace utils{
    // 拷贝工具
    template<typename Input,typename Output>
    void copy(Input first,Input last,Output result){
        while(first!=last){
            *result++=*first++;
        }
    }

    // remove_reference工具
    // 默认情况
    template <typename T>
    struct remove_reference{
        using type=T;
    };

    // 移除左值引用 &
    template <typename T>
    struct remove_reference<T &>{
        using type=T;
    };

    // 移除右值引用 &&
    template <typename T>
    struct remove_reference<T &&>{
        using type=T;
    };

    // 辅助别名模板
    template <typename T>
    using remove_reference_t=typename remove_reference<T>::type;

    // move实现
    template <typename T>
    constexpr auto move(T &&t) noexcept{
        return static_cast<remove_reference_t<T> &&>(t);
    }

    // 移动工具
    template<typename Input,typename Output>
    void move(Input first,Input last,Output result){
        while(first!=last){
            *result++=std::move(*first++);
        }
    }
}

#endif // TOOLS_H