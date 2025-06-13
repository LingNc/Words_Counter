#ifndef TOOLS_H
#define TOOLS_H

namespace utils{
    // 拷贝工具
    /*
        * @brief 拷贝元素从[first,last)到result
        * @tparam Input 输入迭代器类型
        * @tparam Output 输出迭代器类型
        * @param first 输入范围的起始迭代器
        * @param last 输入范围的结束迭代器
        * @param result 输出范围的起始迭代器
    */
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
    /*
        * @brief 将对象转换为右值引用
        * @tparam T 对象类型
        * @param t 要转换的对象
        * @return 返回一个右值引用
        * @note 该函数用于实现移动语义，避免不必要的拷贝。
    */
    template <typename T>
    constexpr auto move(T &&t) noexcept{
        return static_cast<remove_reference_t<T> &&>(t);
    }

    // 移动工具
    /*
        * @brief 移动元素从[first,last)到result
        * @tparam Input 输入迭代器类型
        * @tparam Output 输出迭代器类型
        * @param first 输入范围的起始迭代器
        * @param last 输入范围的结束迭代器
        * @param result 输出范围的起始迭代器
        * @note 该函数用于实现移动语义，避免不必要的拷贝。
    */
    template<typename Input,typename Output>
    void move(Input first,Input last,Output result){
        while(first!=last){
            *result++=utils::move(*first++);
        }
    }

    // 完美转发
    /*
        * @brief 完美转发函数
        * @tparam T 要转发的类型
        * @param t 要转发的对象
        * @return 返回一个右值引用或左值引用，具体取决于传入的参数类型
        * @note 该函数用于实现完美转发，保留参数的值类别。
    */
    template <typename T>
    constexpr T &&forward(utils::remove_reference_t<T> &t) noexcept{
        return static_cast<T &&>(t);
    }

    // 冒泡排序
    template <typename Iter>
    void sort(Iter first,Iter last){
        if(first==last) return; // 空范围
        for(auto it=first; it!=last; ++it){
            for(auto jt=it+1; jt!=last; ++jt){
                if(*jt<*it){
                    auto temp=utils::move(*it);
                    *it=utils::move(*jt);
                    *jt=temp;
                }
            }
        }
    }

    // 交换
    template <typename T>
    void swap(T &a,T &b){
        T temp=utils::move(a);
        a=utils::move(b);
        b=temp;
    }
}

#endif // TOOLS_H