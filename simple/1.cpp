// 编写一个模板元编程实现的阶乘计算。要求使用模板递归的方式，在编译时计算给定整数的阶乘。

#include <type_traits>

template <int N> struct F { constexpr static int value = F<N - 1>::value * N; };

template <> struct F<0> { constexpr static int value = 1; };

int main() {
  static_assert(F<0>::value == 1);
  static_assert(F<1>::value == 1);
  static_assert(F<3>::value == 6);
}