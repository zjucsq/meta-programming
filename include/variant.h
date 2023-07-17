//
// Created by csq on 5/13/23.
//
// from https://zhuanlan.zhihu.com/p/597987861
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

template <typename Ty1, typename... Ty2> struct max_size {
  constexpr static int size = sizeof(Ty1) > max_size<Ty2...>::size
                                  ? sizeof(Ty1)
                                  : max_size<Ty2...>::size;
};

template <typename T> struct max_size<T> {
  constexpr static int size = sizeof(T);
};

template <int idx, typename T, typename... Ts> struct get_ith_type {
  using type = typename get_ith_type<idx - 1, Ts...>::type;
};

template <typename T, typename... Ts> struct get_ith_type<0, T, Ts...> {
  using type = T;
};

template <typename... Ts> struct get_idx;

template <typename T, typename... Ts> struct get_idx<T, T, Ts...> {
  constexpr static int index = 0;
};

template <typename T, typename U, typename... Ts> struct get_idx<T, U, Ts...> {
  constexpr static int index = 1 + get_idx<T, Ts...>::index;
};

template <class T> void destroy(char *data) {
  reinterpret_cast<T *>(data)->~T();
}

template <typename... Ts> struct variant {
  variant() : type(-1) {}

  template <typename T> variant(const T &rhs) {
    *reinterpret_cast<T *>(data) = rhs;
    type = get_idx<T, Ts...>::index;
  }

  // TODO 移动构造和拷贝构造能一起吗？
  template <typename T> variant &operator=(const T &rhs) {
    *reinterpret_cast<T *>(data) = rhs;
    type = rhs.type;
  }

  ~variant() {
    if (type != -1) {
      destroy_func[type](data);
    }
  }

  int index() { return type; }

  template <int id> auto &get() {
    using T = typename get_ith_type<id, Ts...>::type;
    return *reinterpret_cast<T *>(data);
  }

  template <typename T> auto &get() { return *reinterpret_cast<T *>(data); }

private:
  int type;
  using destroy_func_t = void (*)(char *);
  constexpr static destroy_func_t destroy_func[] = {destroy<Ts>...};
  char data[max_size<Ts...>::size] = {0};
};
