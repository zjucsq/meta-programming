// 编写一个模板元编程程序，实现编译时的类型合并，将两个类型列表（如std::tuple）合并为一个新的类型列表，其中重复的类型只保留一个。要求：
// 1. 使用模板元编程技术实现。
// 2. 类型合并需要在编译时完成。
// 3. 示例输出：对于类型列表 std::tuple<int, float, char> 和 std::tuple<char,
// double, int>，合并后的类型列表应为 std::tuple<int, float, char, double>。
// 4. 实现一个通用的合并器，可以应用于任何类型列表

#include <iostream>
#include <tuple>
#include <type_traits>

template <typename L, typename T> struct In {};

template <template <typename...> typename List, typename Hd, typename... Tl,
          typename T>
struct In<List<Hd, Tl...>, T>
    : std::conditional_t<std::is_same_v<Hd, T>, std::true_type,
                         In<List<Tl...>, T>> {};

template <template <typename...> typename List, typename T>
struct In<List<>, T> : std::false_type {};

template <typename L1, typename L2> struct Merge {};

template <template <typename...> typename List, typename... Ts1, typename Hd,
          typename... Tl>
struct Merge<List<Ts1...>, List<Hd, Tl...>> {
  using type =
      std::conditional_t<In<List<Ts1...>, Hd>::value,
                         typename Merge<List<Ts1...>, List<Tl...>>::type,
                         typename Merge<List<Ts1..., Hd>, List<Tl...>>::type>;
};

template <template <typename...> typename List, typename... Ts1>
struct Merge<List<Ts1...>, List<>> {
  using type = List<Ts1...>;
};

template <typename... Args>
void print_tuple_types(const std::tuple<Args...> &t) {
  std::cout << "Tuple types: ";
  std::apply(
      [](const auto &...args) {
        ((std::cout << typeid(args).name() << " "), ...);
      },
      t);
  std::cout << std::endl;
}

int main() {
  // 将类型列表 std::tuple<int, float, char> 和 std::tuple<char, double, int>
  // 合并为一个新的类型列表
  using TypeList1 = std::tuple<int, float, char>;
  using TypeList2 = std::tuple<char, double, int>;
  using MergedTypeList = Merge<TypeList1, TypeList2>::type;

  static_assert(
      std::is_same_v<MergedTypeList, std::tuple<int, float, char, double>>);

  // 输出原始类型列表和合并后的类型列表，以验证合并是否正确
  std::cout << "Type list 1: " << typeid(TypeList1).name() << '\n';
  std::cout << "Type list 2: " << typeid(TypeList2).name() << '\n';
  std::cout << "Merged type list: " << typeid(MergedTypeList).name() << '\n';

  print_tuple_types(TypeList1{});
  print_tuple_types(TypeList2{});
  print_tuple_types(MergedTypeList{});

  return 0;
}