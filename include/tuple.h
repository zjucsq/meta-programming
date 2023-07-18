//
// Created by csq on 5/13/23.
//
// from https://zhuanlan.zhihu.com/p/596673583
#include <iostream>
#include <string>
#include <utility>

template <typename... T> struct tuple;

template <> struct tuple<> {};

template <typename Ty1, typename... Ty2>
struct tuple<Ty1, Ty2...> : tuple<Ty2...> {
  Ty1 val;
  using Base = tuple<Ty2...>;

  tuple(Ty1 v, Ty2... args) : val(v), Base(args...) {}

  Ty1 get() { return val; }
};

template <int idx, typename tuple_> struct get_type {
  using type = typename get_type<idx - 1, typename tuple_::Base>::type;
};

template <typename tuple_> struct get_type<0, tuple_> {
  using type = tuple_;
};

template <int idx, typename tuple_> auto &get(tuple_ &t) {
  using type = typename get_type<idx, tuple_>::type;
  return static_cast<type &>(t).val;
}
