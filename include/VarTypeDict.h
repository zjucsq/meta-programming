#include "utils.h"
#include <Sequential.h>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

struct NullParameter;

template <typename... TParameters> struct VarTypeDict {
  template <typename... TTypes> struct Values {
  public:
    using Keys = VarTypeDict;

    template <typename TKey>
    using ValueType =
        Sequential::At<Values, Sequential::Order<VarTypeDict, TKey>>;

    template <typename TKey>
    constexpr static bool IsValueEmpty =
        std::is_same_v<ValueType<TKey>, NullParameter>;

    Values() = default;

    Values(Values &&val) {
      for (size_t i = 0; i < sizeof...(TTypes); ++i) {
        m_tuple[i] = std::move(val.m_tuple[i]);
      }
    }

    // Values<TTypes> and Values<TTypes2> are not same class, so cannot access
    // private member.
    template <typename... TTypes2> friend class Values;
    template <typename... TTypes2> Values(Values<TTypes2...> &&val) {
      static_assert(sizeof...(TTypes) == sizeof...(TTypes2));
      for (size_t i = 0; i < sizeof...(TTypes); ++i) {
        m_tuple[i] = std::move(val.m_tuple[i]);
      }
    }

    template <typename... TTags, typename... TParams>
    void Update(TParameters... p_params);

    template <typename TTag, typename TVal> auto Set(TVal &&val) && {
      constexpr static auto TagPos = Sequential::Order<VarTypeDict, TTag>;

      using rawVal = RemConstRef<TVal>;
      rawVal *tmp = new rawVal(std::forward<TVal>(val));
      m_tuple[TagPos] = std::shared_ptr<void>(tmp, [](void *ptr) {
        rawVal *nptr = static_cast<rawVal *>(ptr);
        delete nptr;
      });

      if constexpr (std::is_same_v<rawVal, Sequential::At<Values, TagPos>>) {
        return *this;
      } else {
        using new_type = Sequential::Set<Values, TagPos, rawVal>;
        return new_type{std::move(*this)};
        // return new_type(std::move(m_tuple));
      }
    }

    template <typename TTag> const auto &Get() const {
      constexpr static auto TagPos = Sequential::Order<VarTypeDict, TTag>;
      using Type = Sequential::At<Values, TagPos>;
      return *static_cast<Type *>(m_tuple[TagPos].get());
    }

    template <typename TTag> auto &Get() {
      constexpr static auto TagPos = Sequential::Order<VarTypeDict, TTag>;
      using Type = Sequential::At<Values, TagPos>;
      return *static_cast<Type *>(m_tuple[TagPos].get());
    }

  private:
    std::shared_ptr<void> m_tuple[sizeof...(TTypes)];
  };

  static auto Create() {
    using type =
        Sequential::Create<Values, NullParameter, sizeof...(TParameters)>;
    return type{};
  }
};