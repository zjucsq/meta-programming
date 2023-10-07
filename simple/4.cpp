// 编写一个模板元编程实现的类型列表。要求实现以下功能：
// 1. 计算类型列表的长度。
// 2. 在类型列表中查找指定类型的位置。
// 3. 在类型列表中插入或删除指定类型。

#include <type_traits>

template <typename... T> struct TypeList {
  constexpr static int length = sizeof...(T);
};

template <typename L>
concept List = requires(L l) {
  l.length;
};

template <List L> struct Length { constexpr static int value = L::length; };

template <typename T, List L, int index = 0> struct Find {};

template <typename T, typename Hd, typename... Tl, int index>
struct Find<T, TypeList<Hd, Tl...>, index>
    : std::conditional_t<std::is_same_v<T, Hd>,
                         std::integral_constant<int, index>,
                         Find<T, TypeList<Tl...>, index + 1>> {};

template <typename T, int index> struct Find<T, TypeList<>, index> {
  constexpr static int value = -1;
};

int main() {
  using MyTypes = TypeList<int, float, double, char>;

  static_assert(Length<MyTypes>::value == 4);
  static_assert(Find<int, MyTypes>::value == 0);
  static_assert(Find<float, MyTypes>::value == 1);
  static_assert(Find<double, MyTypes>::value == 2);
  static_assert(Find<char, MyTypes>::value == 3);
  static_assert(Find<long, MyTypes>::value == -1);
}