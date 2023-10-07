// 编写一个模板元编程程序，实现编译时的类型排序，将一个类型列表（如
// std::tuple）中的类型按照给定的比较函数进行排序。要求：
// 1. 使用模板元编程技术实现。
// 2. 类型排序需要在编译时完成。
// 3. 示例输出：对于类型列表 std::tuple<double, char, float, int> 和比较函数
// SizeComparator（按照类型的字节数进行排序），排序后的类型列表应为
// std::tuple<char, int, float, double>。
// 4. 实现一个通用的类型排序器，可以应用于任何类型列表和比较函数。

#include "TypeList.h"
#include <tuple>
#include <type_traits>

template <typename T1, typename T2>
struct SizeComparator : std::bool_constant<sizeof(T1) < sizeof(T2)> {};

template <typename L, typename T> struct Append {};

template <template <typename...> typename TypeList, typename... Ts, typename T>
struct Append<TypeList<Ts...>, T> {
  using type = TypeList<Ts..., T>;
};

template <typename L1, typename L2> struct Concat {};

template <template <typename...> typename TypeList, typename... Ts1,
          typename... Ts2>
struct Concat<TypeList<Ts1...>, TypeList<Ts2...>> {
  using type = TypeList<Ts1..., Ts2...>;
};

template <typename L, template <typename> typename Pred> struct Partition {};

template <template <typename...> typename TypeList, typename Hd, typename... Tl,
          template <typename> class Pred>
struct Partition<TypeList<Hd, Tl...>, Pred> {
  using lower = std::conditional_t<
      Pred<Hd>::value,
      typename Concat<TypeList<Hd>,
                      typename Partition<TypeList<Tl...>, Pred>::lower>::type,
      typename Partition<TypeList<Tl...>, Pred>::lower>;
  using higher = std::conditional_t<
      Pred<Hd>::value, typename Partition<TypeList<Tl...>, Pred>::higher,
      typename Concat<TypeList<Hd>,
                      typename Partition<TypeList<Tl...>, Pred>::higher>::type>;
};

template <template <typename...> typename TypeList,
          template <typename> class Pred>
struct Partition<TypeList<>, Pred> {
  using lower = TypeList<>;
  using higher = TypeList<>;
};

template <typename L, template <typename, typename> typename Comparator>
struct Sort {};

template <template <typename...> typename TypeList, typename Hd, typename... Tl,
          template <typename, typename> class Comparator>
struct Sort<TypeList<Hd, Tl...>, Comparator> {
private:
  template <typename T> struct Pred : SizeComparator<T, Hd> {};
  using P = Partition<TypeList<Tl...>, Pred>;
  using LowerSortered = typename Sort<typename P::lower, Comparator>::type;
  using HigherSortered = typename Sort<typename P::higher, Comparator>::type;

public:
  using type = typename Concat<typename Append<LowerSortered, Hd>::type,
                               HigherSortered>::type;
};

template <template <typename...> typename TypeList, typename Hd,
          template <typename, typename> class Comparator>
struct Sort<TypeList<Hd>, Comparator> {
  using type = TypeList<Hd>;
};

template <template <typename...> typename TypeList,
          template <typename, typename> class Comparator>
struct Sort<TypeList<>, Comparator> {
  using type = TypeList<>;
};

int main() {
  // 将类型列表 std::tuple<double, char, float, int[5], double[5]>
  // 按照类型的字节数进行排序
  using TypeList = std::tuple<double, char, float, int[5], double[5]>;
  using SortedTypeList = Sort<TypeList, SizeComparator>::type;
  static_assert(std::is_same_v<std::tuple<char, int>,
                               Append<std::tuple<char>, int>::type>);
  static_assert(std::is_same_v<std::tuple<char>,
                               Concat<std::tuple<char>, std::tuple<>>::type>);
  static_assert(
      std::is_same_v<SortedTypeList,
                     std::tuple<char, float, double, int[5], double[5]>>);
}