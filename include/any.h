//
// Created by csq on 5/14/23.
//
// from https://zhuanlan.zhihu.com/p/597223487
// C++的那些事——any类 - 船长J.K的文章 - 知乎 https://zhuanlan.zhihu.com/p/589164471
// C/C++杂谈：动手写个any - 月踏的文章 - 知乎 https://zhuanlan.zhihu.com/p/386392397
// https://zhuanlan.zhihu.com/p/394827450
// https://wanghenshui.github.io/2021/02/06/any.html
#include <any>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

class any {
   public:
    any()
        : value_(nullptr),
          getType_(([]() -> const std::type_info & { return typeid(void); })),
          clone_(nullptr),
          destroy_(nullptr) {}

    template <typename T>
    any(const T &value)
        : value_(static_cast<void *>(new T{value})),
          getType_([]() -> const std::type_info & { return typeid(T); }),
          clone_([](void *otherData) { return static_cast<void *>(new T{*static_cast<T *>(otherData)}); }),
          destroy_([](void *data) { delete static_cast<T *>(data); }) {}

    any(const any &other)
        : value_(other.clone_(other.value_)),
          getType_(other.getType_),
          clone_(other.clone_),
          destroy_(other.destroy_) {}

    //    any &operator=(const any &other) {
    //        if (this != &other) {
    //            if (destroy_)
    //                destroy_(value_);
    //            value_ = other.clone_(other.value_);
    //            getType_ = other.getType_;
    //            clone_ = other.clone_;
    //            destroy_ = other.destroy_;
    //        }
    //        return *this;
    //    }

    any &operator=(const any &other) {
        if (this != &other) {
            any temp(other);
            swap(temp);
        }
        return *this;
    }

    any(any &&other) noexcept
        : value_(other.value_), getType_(other.getType_), clone_(other.clone_), destroy_(other.destroy_) {
        other.clear();
    }

    any &operator=(any &&other) noexcept {
        if (this != &other) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    ~any() {
        if (destroy_) destroy_(value_);
    }

    template <typename T>
    T &any_cast() {
        if (typeid(T) == getType_()) {
            return *static_cast<T *>(value_);
        } else {
            throw std::bad_cast();
        }
    }

    const std::type_info &type() const { return getType_(); }

    bool has_value() { return getType_() != typeid(void); }

   private:
    // const std::type_info &type_;
    void *value_;

    using getType_func = const std::type_info &(*)();
    using clone_func = void *(*)(void *);
    using destroy_func = void (*)(void *);
    getType_func getType_;
    clone_func clone_;
    destroy_func destroy_;

    void swap(any &other) noexcept {
        std::swap(value_, other.value_);
        std::swap(getType_, other.getType_);
        std::swap(clone_, other.clone_);
        std::swap(destroy_, other.destroy_);
    }

    void clear() noexcept {
        value_ = nullptr;
        getType_ = []() -> const std::type_info & { return typeid(void); };
        clone_ = nullptr;
        destroy_ = nullptr;
    }
};
