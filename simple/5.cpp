// 编写一个模板元编程实现的编译时数组。要求实现以下功能：
// 1. 计算数组的大小。
// 2. 访问数组中指定位置的元素。
// 3. 修改数组中指定位置的元素。

#include <type_traits>

template <typename T, T... Values> struct CompileTimeArray {
  using ValueType = T;
  static constexpr std::size_t Size = sizeof...(Values);
  static constexpr T Data[] = {Values...};
};

template <int Index, typename T, T... Values>
constexpr T Get(CompileTimeArray<T, Values...>) {
  static_assert(Index < sizeof...(Values), "Index out of range");
  return CompileTimeArray<T, Values...>::Data[Index];
};

int main() {
  using MyArray = CompileTimeArray<int, 1, 2, 3, 4, 5>;

  static_assert(MyArray::Size == 5);
  static_assert(Get<0>(MyArray{}) == 1);
  static_assert(Get<2>(MyArray{}) == 3);
  static_assert(Get<MyArray::Size - 1>(MyArray{}) == 5);
}