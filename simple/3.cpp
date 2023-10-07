// 使用模板元编程 判断某个类是否有某个变量

#include <type_traits>

template <typename T> class has_value_member0 {
  template <typename C> static char test(decltype(&C::value));
  template <typename C> static int test(...);

public:
  static constexpr bool value = (sizeof(test<T>(0)) == sizeof(char));
};

template <typename T, typename = void>
struct has_value_member1 : std::false_type {};

template <typename T>
struct has_value_member1<T, std::void_t<decltype(T::value)>> : std::true_type {
};

// 这是判断有没有一个类型名字叫做value，比如using value = int
// template <typename T>
// struct has_value_member1<T, std::void_t<typename T::value>> : std::true_type
// {};

template <typename T, typename = void>
struct has_value_member2 : std::false_type {};

template <typename T>
struct has_value_member2<
    T, std::enable_if_t<std::is_same_v<decltype(T::value), decltype(T::value)>>>
    : std::true_type {};

template <typename T>
concept has_value_member3 = requires(T t) {
  t.value;
  // typename T::value;
};

template <typename T>
struct has_value_member4 : std::bool_constant<has_value_member3<T>> {};

// 示例类
struct A {
  int value;
};

struct B {
  float other_value;
};

int main() {
  static_assert(has_value_member0<A>::value);
  static_assert(!has_value_member0<B>::value);
  static_assert(has_value_member1<A>::value);
  static_assert(!has_value_member1<B>::value);
  static_assert(has_value_member2<A>::value);
  static_assert(!has_value_member2<B>::value);
  static_assert(has_value_member3<A>);
  static_assert(!has_value_member3<B>);
  static_assert(has_value_member4<A>::value);
  static_assert(!has_value_member4<B>::value);
}