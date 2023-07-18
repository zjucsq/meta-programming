#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

// Naive implementation, by inheritance
template <typename Ret, typename... Args> class function_a;

template <typename Ret, typename... Args> class function_a<Ret(Args...)> {
public:
  template <typename T> function_a &operator=(T t) {
    callable_ = std::make_unique<CallableT<T>>(t);
    return *this;
  }
  Ret operator()(Args... args) const {
    assert(callable_);
    return callable_->Invoke(args...);
  }

private:
  class ICallable {
  public:
    virtual ~ICallable() = default;
    virtual Ret Invoke(Args...) = 0;
  };

  template <typename T> class CallableT : public ICallable {
  public:
    CallableT(const T &t) : t_(t) {}
    ~CallableT() override = default;
    Ret Invoke(Args... args) override { return t_(args...); }

  private:
    T t_;
  };
  std::unique_ptr<ICallable> callable_;
};

// Aother implementation
template <typename Ret, typename... Args> class function_b;

template <typename Ret, typename... Args> class function_b<Ret(Args...)> {
private:
  using invoke_fn_t = Ret (*)(char *, Args &&...);
  using construct_fn_t = void (*)(char *, char *);
  using destory_fn_t = void (*)(char *);

  template <typename F> static Ret invoke_fn(F *fn, Args &&...args) {
    return (*fn)(std::forward<Args>(args)...);
  }
  template <typename F> static void construct_fn(F *dst, F *src) {
    new (dst) F(*src);
  }
  template <typename F> static void destroy_fn(F *f) { f->~F(); }

  invoke_fn_t invoke_f;
  construct_fn_t construct_f;
  destory_fn_t destory_f;

  std::unique_ptr<char[]> data_ptr;
  size_t data_size;

public:
  function_b()
      : invoke_f(nullptr), construct_f(nullptr), destory_f(nullptr),
        data_ptr(nullptr), data_size(0) {}

  template <typename F>
  function_b(F f)
      : invoke_f(reinterpret_cast<invoke_fn_t>(invoke_fn<F>)),
        construct_f(reinterpret_cast<construct_fn_t>(construct_fn<F>)),
        destory_f(reinterpret_cast<destory_fn_t>(destroy_fn<F>)),
        data_ptr(new char[sizeof(F)]), data_size(sizeof(F)) {
    construct_f(data_ptr.get(), reinterpret_cast<char *>(&f));
  }

  function_b(const function_b &rhs)
      : invoke_f(rhs.invoke_f), construct_f(rhs.construct_f),
        destory_f(rhs.destory_f), data_size(rhs.data_size) {
    if (rhs.invoke_f) {
      data_ptr.reset(new char[data_size]);
      construct_f(data_ptr.get(), rhs.data_ptr.get());
    }
  }

  function_b &operator=(const function_b &rhs) {
    if (this != &rhs) {
      invoke_f = rhs.invoke_f;
      construct_f = rhs.construct_f;
      destory_f = rhs.destory_f;
      data_size = rhs.data_size;
      if (rhs.invoke_f) {
        data_ptr.reset(new char[data_size]);
        construct_f(data_ptr.get(), rhs.data_ptr.get());
      }
    }
    return *this;
  }

  ~function_b() {
    if (data_ptr)
      destory_f(data_ptr.get());
  }

  Ret operator()(Args &&...args) {
    return invoke_f(data_ptr.get(), std::forward<Args>(args)...);
  }
};

// A better implementation
// https://zhuanlan.zhihu.com/p/66301236
template <typename Ret, typename... Args> class function_c;

template <typename Ret, typename... Args> class function_c<Ret(Args...)> {
public:
  function_c() : callable_(nullptr) {}
  template <typename F> function_c(F &&f) {
    if (sizeof(CallableT<F>) <= sizeof(buf)) {
      callable_ = new (&buf) CallableT<F>(std::forward<F>(f));
    } else {
      callable_ = new CallableT<F>(std::forward<F>(f));
    }
  }
  template <typename F> function_c &operator=(F &&f) {
    if (sizeof(CallableT<F>) <= sizeof(buf)) {
      callable_ = new (&buf) CallableT<F>(std::forward<F>(f));
    } else {
      callable_ = new CallableT<F>(std::forward<F>(f));
    }
    return *this;
  }
  ~function_c() {
    if (callable_) {
      // callable_->destroy();
      if ((void *)callable_ != (void *)&buf) {
        delete callable_;
      }
    }
  }

  Ret operator()(Args... args) const {
    assert(callable_);
    return callable_->Invoke(args...);
  }

private:
  class ICallable {
  public:
    virtual ~ICallable() = default;
    virtual Ret Invoke(Args...) = 0;
    virtual ICallable *clone() const = 0;
    virtual void clone(ICallable *) const = 0;
    // virtual void destroy() noexcept = 0;
    // virtual void destroy_deallocate() noexcept = 0;
  };

  template <typename T> class CallableT : public ICallable {
  public:
    CallableT(const T &t) : t_(t) {}
    CallableT(T &&t) : t_(std::move(t)) {}
    ~CallableT() override {
      if (std::is_destructible_v<T>)
        t_.~T();
    }
    Ret Invoke(Args... args) override { return t_(args...); }
    ICallable *clone() const override { return new CallableT<T>(t_); }
    void clone(ICallable *p) const override { p = new CallableT<T>(t_); }
    // void destroy() noexcept override {
    //   if constexpr (std::is_destructible_v<T>)
    //     t_.~T();
    // }
    // void destroy_deallocate() noexcept override { t_.~T(); ~CallableT(); }

  private:
    T t_;
  };

  char buf[sizeof(void *) * 3];
  ICallable *callable_;
};

// Aother implementation
// template <typename F> class function_d;

// template <typename Ret, typename... Args> class function_d<Ret(Args...)> {
// private:
//   union policy_storage {
//     mutable char small[sizeof(void *) * 2];
//     void *large;
//   };

//   template <typename F>
//   struct use_small_storage
//       : public std::integral_constant<
//             bool, sizeof(F) <= sizeof(policy_storage) &&
//                       alignof(F) <= alignof(policy_storage) &&
//                       std::is_trivially_copy_constructible_v<F> &&
//                       std::is_trivially_destructible_v<F>> {};

//   template <typename _Tp>
//   using fast_forward =
//       typename std::conditional<std::is_scalar<_Tp>::value, _Tp, _Tp
//       &&>::type;

//   template <typename F> struct policy_invoker;

//   template <typename Ret_, typename... Args_>
//   struct policy_invoker<Ret_(Args_...)> {
//     using Call = Ret_ (*)(const policy_storage *, fast_forward<Args>...);
//     Call call;

//     policy_invoker() : call(nullptr) {}

//     template <typename F> static policy_invoker create() {
//       return policy_invoker(&call_impl<F>);
//     }

//   private:
//     explicit policy_invoker(Call c) : call(c) {}

//     // static Ret call_empty(const policy_storage *buf, Args... args) {
//     //   throw bad_function_call();
//     // }

//     template <typename F>
//     static Ret call_impl(const policy_storage *buf, Args... args) {
//       F *f = reinterpret_cast<F *>(use_small_storage<F>::value ? &buf->small
//                                                                : buf->large);
//       return f(fast_forward<Args>(args)...);
//     }
//   };

//   struct policy {
//     using clone = void (*)();
//     using destroy = void (*)();

//     template <typename F> policy &create() {}

//   private:
//     void clone_small()
//   };

//   policy_storage buf;
//   using invoker = policy_invoker<Ret(Args)...>;
//   invoker invoker_;
//   const policy *policy_;

// public:
//   function_d()
//       : invoke_f(nullptr), construct_f(nullptr), destory_f(nullptr),
//         data_ptr(nullptr), data_size(0) {}

//   template <typename F>
//   function_d(F f)
//       : invoke_f(reinterpret_cast<invoke_fn_t>(invoke_fn<F>)),
//         construct_f(reinterpret_cast<construct_fn_t>(construct_fn<F>)),
//         destory_f(reinterpret_cast<destory_fn_t>(destroy_fn<F>)),
//         data_ptr(new char[sizeof(F)]), data_size(sizeof(F)) {
//     construct_f(data_ptr.get(), reinterpret_cast<char *>(&f));
//   }

//   function_d(const function_d &rhs)
//       : invoke_f(rhs.invoke_f), construct_f(rhs.construct_f),
//         destory_f(rhs.destory_f), data_size(rhs.data_size) {
//     if (rhs.invoke_f) {
//       data_ptr.reset(new char[data_size]);
//       construct_f(data_ptr.get(), rhs.data_ptr.get());
//     }
//   }

//   function_d &operator=(const function_d &rhs) {
//     if (this != &rhs) {
//       invoke_f = rhs.invoke_f;
//       construct_f = rhs.construct_f;
//       destory_f = rhs.destory_f;
//       data_size = rhs.data_size;
//       if (rhs.invoke_f) {
//         data_ptr.reset(new char[data_size]);
//         construct_f(data_ptr.get(), rhs.data_ptr.get());
//       }
//     }
//     return *this;
//   }

//   ~function_d() {
//     if (data_ptr)
//       destory_f(data_ptr.get());
//   }

//   Ret operator()(Args &&...args) {
//     return invoke_f(data_ptr.get(), std::forward<Args>(args)...);
//   }
// };