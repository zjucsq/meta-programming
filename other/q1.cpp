//
// Created by csq on 5/10/23.
//

// 写一个函数模板，对于支持-运算符的类，调用-运算符，否则调用abstract
#include <iostream>
#include <type_traits>

class A {
public:
  int num;

  A(int num) : num(num) {}

  A operator-(const A &a) const { return A(num - a.num); }
};

class B {
public:
  int num;

  B(int num) : num(num) {}

  B subtract(const B &a) const { return B(num - a.num); }
};

template <class, class = void> struct has_sub : std::false_type {};

template <class T>
struct has_sub<T, std::void_t<decltype(std::declval<T>() - std::declval<T>())>>
    : std::true_type {};

template <typename T> T subImpl(T a, T b, std::true_type) {
  std::cout << "call - ";
  return a - b;
}

template <typename T> T subImpl(T a, T b, std::false_type) {
  std::cout << "call subtract ";
  return a.subtract(b);
}

template <typename T> T sub1(T a, T b) { return subImpl(a, b, has_sub<T>{}); }

template <typename T, std::enable_if_t<has_sub<T>{}> * = nullptr>
T sub2(T a, T b) {
  std::cout << "call - ";
  return a - b;
}

template <typename T, std::enable_if_t<!has_sub<T>{}> * = nullptr>
T sub2(T a, T b) {
  std::cout << "call subtract ";
  return a.subtract(b);
}

template <typename T>
T sub3(T a, T b, decltype(std::declval<T>() - std::declval<T>()) * = nullptr) {
  std::cout << "call - ";
  return a - b;
}

template <typename T> T sub3(T a, T b, ...) {
  std::cout << "call subtract ";
  return a.subtract(b);
}

int main() {
  std::cout << sub1(1, 2) << std::endl;
  std::cout << sub1(A{1}, A{2}).num << std::endl;
  std::cout << sub1(B{1}, B{2}).num << std::endl;

  std::cout << sub2(1, 2) << std::endl;
  std::cout << sub2(A{1}, A{2}).num << std::endl;
  std::cout << sub2(B{1}, B{2}).num << std::endl;

  std::cout << sub3(1, 2) << std::endl;
  std::cout << sub3(A{1}, A{2}).num << std::endl;
  std::cout << sub3(B{1}, B{2}).num << std::endl;
}
