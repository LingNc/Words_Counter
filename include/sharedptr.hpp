#ifndef SHARE_PTR_H
#define SHARE_PTR_H

#include <cstddef>
#include "utils.hpp"

namespace utils{
    /*
        * @brief 控制块类，用于管理共享指针的引用计数和对象指针
        * @tparam T 管理的对象类型
        * @note 该类用于实现共享指针的引用计数机制，确保对象在没有引用时被正确销毁。
    */
    template<typename T>
    class Control_Block{
    public:
        // 引用计数
        size_t ref_count=1;
        T *object_ptr=nullptr;

        // 销毁控制对象
        void destroy();
        // 销毁控制块
        void del_cblock();
    };

    // 共享指针类
    template<typename T>
    class shared_ptr{
        T *_data=nullptr;
        Control_Block<T> *_cblock=nullptr;

        // 减少并销毁函数
        void decrease();

    public:
        /*
            * @brief 构造函数，接受一个指针
            * @param p 指向要管理的对象的指针
            * @note 如果传入的指针不为空，将创建一个控制块并初始化对象指针。
            *       如果传入的指针为空，则不创建控制块。
        */
        explicit shared_ptr(T *p=nullptr);

        // 拷贝构造
        shared_ptr(const shared_ptr &other);

        // 移动构造(转移所有权)
        shared_ptr(shared_ptr &&other) noexcept;

        // 析构
        ~shared_ptr();

        // 一些常用的运算符重载
        T &operator*()const;
        T *operator->()const;
        // 重载赋值
        shared_ptr &operator=(const shared_ptr &other) noexcept;

        // 移动赋值
        shared_ptr &operator=(shared_ptr &&other) noexcept;

        // 重载转换到bool类型
        explicit operator bool() const noexcept;

        // 工厂函数 组合分配
        /*
            * @brief 创建一个共享指针，使用组合内存分配
            * @tparam T 对象类型
            * @tparam Args 构造函数参数类型
            * @param args 构造函数参数
            * @return 返回一个shared_ptr<T>对象
            * @note 该函数使用组合内存分配来创建对象，避免了额外的内存分配开销。
        */
        template<typename U,typename... Args>
        friend shared_ptr<U> utils::make_shared(Args &&...args);
    };

    // 组合内存块
    template <typename T>
    struct ComBlock{
        Control_Block<T> _cblock;
        // 对齐内存增加可移植性
        alignas(T) unsigned char _object[sizeof(T)];
    };



    // 以下是共享指针的实现部分

    template<typename U,typename...Args>
    shared_ptr<U> make_shared(Args&&...args){
        // 创建组合块
        auto *block=new ComBlock<U>;
        // 完美转发参数，构建对象
        U *object_ptr=new (&block->object_ptr) U(utils::forward<Args>(args)...);
        // 初始化控制块
        block->_cblock.object_ptr=object_ptr;
        return shared_ptr<U>(object_ptr,&block->_cblock);
    }

    // 销毁控制对象
    template<typename T>
    void Control_Block<T>::destroy(){
        if(object_ptr){
            delete object_ptr;
        }
    }

    // 销毁控制块
    template<typename T>
    void Control_Block<T>::del_cblock(){
        delete this;
    }

    // 减少并销毁函数
    template<typename T>
    void shared_ptr<T>::decrease(){
        // 如果控制块存在且引用计数归零
        if(_cblock&&--_cblock->ref_count==0){
            // 销毁
            _cblock->destroy();
            _cblock->del_cblock();
        }
    }

    // 构造函数
    template<typename T>
    shared_ptr<T>::shared_ptr(T *p): _data(p){
        // 如果传入的指针不为空，创建控制块
        if(_data){
            // 初始化控制块
            _cblock=new Control_Block<T>();
            _cblock->object_ptr=_data;
        }
    }

    // 拷贝构造
    template<typename T>
    shared_ptr<T>::shared_ptr(const shared_ptr &other): _data(other._data), _cblock(other._cblock) {
        if (_cblock) {
            ++_cblock->ref_count;
        }
    }

    // 移动构造(转移所有权)
    template<typename T>
    shared_ptr<T>::shared_ptr(shared_ptr &&other) noexcept : _data(other._data), _cblock(other._cblock) {
        other._data = nullptr;
        other._cblock = nullptr;
    }

    // 析构
    template<typename T>
    shared_ptr<T>::~shared_ptr() {
        decrease();
    }

    // 一些常用的运算符重载
    template<typename T>
    T &shared_ptr<T>::operator*() const {
        return *_data;
    }

    template<typename T>
    T *shared_ptr<T>::operator->() const {
        return _data;
    }

    // 重载赋值
    template<typename T>
    shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr &other) noexcept {
        if (this != &other) {  // 防止自赋值
            // 减少当前引用计数
            decrease();
            // 复制新数据
            _data = other._data;
            _cblock = other._cblock;
            // 增加新的引用
            if (_cblock) {
                ++_cblock->ref_count;
            }
        }
        return *this;
    }

    // 移动赋值
    template<typename T>
    shared_ptr<T> &shared_ptr<T>::operator=(shared_ptr &&other) noexcept {
        if (this != &other) {  // 防止自赋值
            // 减少当前引用计数
            decrease();
            // 转移所有权
            _data = other._data;
            _cblock = other._cblock;
            other._data = nullptr;
            other._cblock = nullptr;
        }
        return *this;
    }

    // 重载转换到bool类型
    template<typename T>
    shared_ptr<T>::operator bool() const noexcept {
        return _data != nullptr;  // 检查指针是否非空
    }
}

#endif // SHARE_PTR_H