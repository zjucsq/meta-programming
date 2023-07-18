//
// Created by csq on 2/23/23.
//

#ifndef META_CHAPTER2_H
#define META_CHAPTER2_H

#include <type_traits>
#include <utility>

#include "utils.h"

// 2.1

// Size
template <typename TArray> struct Size_ {
  constexpr static size_t value = -1;
};

template <template <typename...> class Cout, typename... T>
struct Size_<Cout<T...>> {
  constexpr static size_t value = sizeof...(T);
};

template <typename TArray>
constexpr static size_t Size = Size_<RemConstRef<TArray>>::value;

// At
template <typename TCont, size_t ID> struct At_;

template <template <typename...> class TCont, typename TCurType,
          typename... TTypes, size_t ID>
struct At_<TCont<TCurType, TTypes...>, ID> {
  using type = typename At_<TCont<TTypes...>, ID - 1>::type;
};

template <template <typename...> class TCont, typename TCurType,
          typename... TTypes>
struct At_<TCont<TCurType, TTypes...>, 0> {
  using type = TCurType;
};

// 2.2 optimization

// pack expansion
// template<typename TInCont, template<typename> typename F,
//         template<typename ...> typename TOutCont>
// struct Transform_;

// template<typename >
// Transform_<TInCont<TInputs...>, F, TOutCont>

#endif // META_CHAPTER2_H
