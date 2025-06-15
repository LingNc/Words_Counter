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
    // is_pointer 工具
    template<typename T>
    struct is_pointer{
        static constexpr bool value=false; // 默认不是指针
    };
    template<typename T>
    struct is_pointer<T *>{ // 匹配 T*
        static constexpr bool value=true;
    };

    template<typename T>
    struct is_pointer<T *const>{ // 匹配 const T*
        static constexpr bool value=true;
    };

    template<typename T>
    struct is_pointer<T *volatile>{ // 匹配 volatile T*
        static constexpr bool value=true;
    };

    template<typename T>
    struct is_pointer<T *const volatile>{ // 匹配 const volatile T*
        static constexpr bool value=true;
    };

    // 快捷别名模板
    template<typename T>
    inline constexpr bool is_pointer_v=is_pointer<T>::value;

    // remove_pointer 工具
    template <typename T>
    struct remove_pointer{
        // 默认情况
        using type=T;
    };

    // 特化所有指针类型
    template <typename T>
    struct remove_pointer<T *>{
        using type=T;
    };

    template <typename T>
    struct remove_pointer<T *const>{
        using type=T;
    };

    template <typename T>
    struct remove_pointer<T *volatile>{
        using type=T;
    };

    template <typename T>
    struct remove_pointer<T *const volatile>{
        using type=T;
    };

    template <typename T>
    struct remove_pointer<T *const &>{
        using type=T;
    };

    // 快捷别名模板
    template <typename T>
    using remove_pointer_t=typename remove_pointer<T>::type;

    // conditional 工具
    template<bool B,typename T,typename F>
    struct conditional{ // 基础模板
        using type=T;  // 如果 B=true，选择 T
    };

    template<typename T,typename F>
    struct conditional<false,T,F>{ // 特化版本
        using type=F;  // 如果 B=false，选择 F
    };

    // 别名模板
    template<bool B,typename T,typename F>
    using conditional_t=typename conditional<B,T,F>::type;

    // 获取迭代器类型
    template<typename Iter>
    using iter_value_t=utils::conditional_t<
        utils::is_pointer_v<Iter>,          // 条件：Iter 是否是指针？
        utils::remove_pointer_t<Iter>,      // 如果是指针，移除指针（获取指向的类型）
        typename Iter::value_type         // 否则，取 Iter::value_type
    >;

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

    // less实现
    // 基础版本（比较普通类型）
    /*
        * @brief 比较两个元素是否小于
        * @tparam T 元素类型
        * @param a 第一个元素
        * @param b 第二个元素
        * @return 如果 a < b 返回 true，否则返回 false
        * @note 该函数用于实现默认的比较逻辑。
    */
    template <typename T>
    struct less{
        bool operator()(const T &a,const T &b) const{
            return a<b;
        }
    };

    // 冒泡排序
    template <typename Iter,typename Compare=utils::less<typename utils::iter_value_t<Iter>>>
    void sort(Iter first,Iter last,Compare comp=Compare{}){
        if(first==last) return; // 空范围
        for(auto it=first; it!=last; ++it){
            for(auto jt=it+1; jt!=last; ++jt){
                if(comp(*jt,*it)){
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