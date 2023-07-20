//
// Created by csq on 2/6/23.
//
#include <iostream>
#include <stack>
#include <type_traits>

template <typename T, typename = void> struct has_type : std::false_type {};
template <typename T>
struct has_type<T, std::void_t<typename T::type>> : std::true_type {};

template <class T, size_t N = 10> struct my_array {
  T arr[N];
};

template <class T> struct my_array<T> {
  T arr[2];
};

// template<typename T, typename Cont>
// struct mystack{
//   Cont<T> c;
// }

int main() {
  my_array<int, 5> a;
  std::cout << sizeof(a.arr) / sizeof(int) << std::endl;
  my_array<int> b;
  std::cout << sizeof(b.arr) / sizeof(int) << std::endl;

  std::stack<int> s;
}