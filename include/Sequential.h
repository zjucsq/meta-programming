#include <type_traits>
#include <utility>
#include <utils.h>

namespace Sequential {
// Size
template <typename TArray> struct Size_ {
  constexpr static size_t value = -1;
};

template <template <typename...> class Cout, typename... T>
struct Size_<Cout<T...>> {
  constexpr static size_t value = sizeof...(T);
};

template <typename TArray>
constexpr static size_t Size = Size_<RemConstRef<TArray>>::value;

// At
template <typename TCont, size_t ID> struct At_;

template <template <typename...> class TCont, typename TCurType,
          typename... TTypes, size_t ID>
struct At_<TCont<TCurType, TTypes...>, ID> {
  using type = typename At_<TCont<TTypes...>, ID - 1>::type;
};

template <template <typename...> class TCont, typename TCurType,
          typename... TTypes>
struct At_<TCont<TCurType, TTypes...>, 0> {
  using type = TCurType;
};

template <typename TCont, size_t ID> using At = typename At_<TCont, ID>::type;

// Order
template <typename TCont, typename T> struct Order_;

template <template <typename...> class TCont, typename... TTypes, typename T>
struct Order_<TCont<T, TTypes...>, T> {
  constexpr static int value = 0;
};

template <template <typename...> class TCont, typename TCurType,
          typename... TTypes, typename T>
struct Order_<TCont<TCurType, TTypes...>, T> {
  constexpr static int value = 1 + Order_<TCont<TTypes...>, T>::value;
};

template <typename TCont, typename T>
constexpr static int Order = Order_<TCont, T>::value;

// Create
template <template <typename...> typename TCont, typename T, size_t N,
          typename... TTypes>
struct Create_ {
  using type = typename Create_<TCont, T, N - 1, T, TTypes...>::type;
};

template <template <typename...> typename TCont, typename T, typename... TTypes>
struct Create_<TCont, T, 0, TTypes...> {
  using type = TCont<TTypes...>;
};

template <template <typename...> typename TCont, typename T, size_t N>
using Create = typename Create_<TCont, T, N>::type;

// Set
template <typename TCont, size_t N, typename T, typename... TPrev> struct Set_;

template <template <typename...> class TCont, size_t N, typename T,
          typename TCurType, typename... TTypes, typename... TPrev>
struct Set_<TCont<TCurType, TTypes...>, N, T, TPrev...> {
  using type =
      typename Set_<TCont<TTypes...>, N - 1, T, TCurType, TPrev...>::type;
};

template <template <typename...> class TCont, typename T, typename TCurType,
          typename... TTypes, typename... TPrev>
struct Set_<TCont<TCurType, TTypes...>, 0, T, TPrev...> {
  using type = TCont<TPrev..., T, TTypes...>;
};

template <typename TCont, size_t N, typename T>
using Set = typename Set_<TCont, N, T>::type;

} // namespace Sequential