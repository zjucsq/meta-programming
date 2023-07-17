//
// Created by csq on 2/23/23.
//

#ifndef META_CHAPTER1_H
#define META_CHAPTER1_H

#include <type_traits>
#include <utility>

// 1.1
// meta-function: 1) a mapping; 2) no side effects; 3) can be invoked in compile-time
constexpr int fun(int a) { return a + 1; }

// using template instead of constexpr (after C++14)
template<int a>
constexpr int fun_1 = a + 1;

// type meta-function
template<typename T>
struct Fun_ {
    using type = T;
};

template<>
struct Fun_<int> {
    using type = unsigned int;
};

template<>
struct Fun_<long> {
    using type = unsigned long;
};

// input: T output: Fun<T>
template<typename T>
struct Fun {
};

// Nonparametric meta-function
struct Fun1 {
    using type = int;
};

constexpr int fun() {
    return 10;
}

// meta-function with more than one return-value
template<typename T>
struct Fun2 {
    using reference_type = T &;
    using const_reference_type = const T &;
    using value_type = T;
};

// 1.2

// use template as input of meta-function
template<template<class name> class T1, typename T2>
struct Fun2_ {
    using type = typename T1<T2>::type;
};

template<template<class name> class T1, typename T2>
using Fun3 = typename T1<T2>::type;

// use template as output of meta-function
template<bool AddOrRemoveRef>
struct Fun3_;

template<>
struct Fun3_<true> {
    template<class T>
    using type = std::add_lvalue_reference<T>;
};

template<>
struct Fun3_<false> {
    template<class T>
    using type = std::remove_reference<T>;
};

// container: variadic template
template<int... Vals>
struct IntContainer;
template<bool... Vals>
struct BoolContainer;

template<typename ... Types>
struct TypeContainer;

template<template<class> class... T>
struct TemplateContainer;
template<template<class...> class... T>
struct TemplateContainer2;

// 1.3

// meta-object and meta-scope

template<typename T, size_t N>
struct Fun4_ {
private:
    constexpr static size_t val = N > 10 ? N / 2 : N;
public:
    using ArrType = T[val];
};

// meta-method
template<typename T>
struct Wrapper {
    template<size_t N>
    using method = T[N];
};

template<typename T>
struct Fun5_ {
    using type = Wrapper<std::remove_pointer_t<T>>;
};

template<typename T>
using Fun5 = typename Fun5_<T>::type;

using res = Fun5<int *>::method<5>;

// 1.4

// sequence code
template<typename T>
struct RemoveReferenceConst_ {
    // note: cannot exchange order between inter_type and type.
private:
    using inter_type = typename std::remove_reference_t<T>;
public:
    using type = typename std::remove_const_t<inter_type>;
};

template<typename T>
using RemoveReferenceConst = typename RemoveReferenceConst_<T>::type;

RemoveReferenceConst<const int &> h = 3;

// branch code

// 1) std::conditional std::conditional_t
namespace my_std {
    template<bool B, typename T, typename F>
    struct conditional {
        using type = T;
    };

    template<typename T, typename F>
    struct conditional<false, T, F> {
        using type = F;
    };

    template<bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;
}

// 2) partial template specialization
struct A;
struct B;

template<typename T>
struct Fun6_ {
    constexpr static size_t value = 0;
};

template<>
struct Fun6_<A> {
    constexpr static size_t value = 1;
};

template<>
struct Fun6_<B> {
    constexpr static size_t value = 2;
};

constexpr size_t h1 = Fun6_<B>::value;

// 3) std::enable_if and std::enable_if_t
// Another similar solution is std::void_t (C++17), see
// 在C++中优雅地检测类型/表达式有效性： void_t & is_detected - SuperSodaSea的文章 - 知乎 https://zhuanlan.zhihu.com/p/26155469
namespace my_std {
    template<bool B, typename T = void>
    struct enable_if {
    };

    template<typename T>
    struct enable_if<true, T> {
        using type = T;
    };

    template<bool B, typename T = void>
    using enable_if_t = typename enable_if<B, T>::type;
}

// 4) Compile-time branch
template<bool Check, std::enable_if_t<Check> * = nullptr>
auto fun() {
    return (int) 0;
}

template<bool Check, std::enable_if_t<!Check> * = nullptr>
auto fun() {
    return (double) 0;
}

template<bool Check>
auto wrap2() {
    return fun<Check>();
}
// wrap2<true>() returns int, wrap2<false>() returns double.

// 5) Compile-time branch, simplify in C++17 by if constexpr
template<bool Check>
auto fun() {
    if constexpr (Check) {
        return (int) 0;
    } else {
        return (double) 0;
    }
}

// loop code

// 1) recursion
template<size_t ...Inputs>
constexpr size_t Accumulate = 0;

template<size_t CutInput, size_t ...Inputs>
constexpr size_t Accumulate<CutInput, Inputs...> = CutInput + Accumulate<Inputs...>;

// 2) Fold Expressions, C++17
template<size_t ... values>
constexpr size_t fun() {
    return (0 + ... + values);
}

// 3) Parameter pack expansion, C++11
template<size_t... I>
struct Cont;

template<size_t ... I>
using Fun6 = Cont<(I + 1)...>;

// compile-time switch
template<typename TBooleanCont, typename TFunCont>
struct CompileTimeSwitch_;

template<typename T>
struct Identity_ {
    using type = T;
};

template<bool curBool, bool ... TBools, template<typename ...> class TFunCont, typename curFunc, typename ...TFuncs>
struct CompileTimeSwitch_<std::integer_sequence<bool, curBool, TBools...>, TFunCont<curFunc, TFuncs...>> {
    static_assert((sizeof...(TBools) == sizeof...(TFuncs)) || (sizeof...(TBools) + 1 == sizeof...(TFuncs)));
    using type = typename std::conditional_t<curBool, Identity_<curFunc>, CompileTimeSwitch_<std::integer_sequence<bool, TBools...>, TFunCont<TFuncs...>>>::type;
};

template<template<typename ...> class TFunCont, typename curFunc>
struct CompileTimeSwitch_<std::integer_sequence<bool>, TFunCont<curFunc>> {
    using type = curFunc;
};

template<typename TBooleanCont, typename TFunCont>
using CompileTimeSwitch = typename CompileTimeSwitch_<TBooleanCont, TFunCont>::type;

// 1.5 CRTP



#endif //META_CHAPTER1_H
