// 编写一个模板元编程实现的斐波那契数列计算。要求使用模板递归的方式，在编译时计算斐波那契数列的第
// N 项。

#include <type_traits>

template <int N> struct Fib {
  constexpr static int value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <> struct Fib<0> { constexpr static int value = 0; };

template <> struct Fib<1> { constexpr static int value = 1; };

int main() {
  static_assert(Fib<0>::value == 0);
  static_assert(Fib<1>::value == 1);
  static_assert(Fib<10>::value == 55);
}