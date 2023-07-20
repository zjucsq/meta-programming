#include <VarTypeDict.h>
#include <iostream>

struct FParams : public VarTypeDict<struct A, struct B, struct Weight> {};

template <typename TIn> float fun(const TIn &in) {
  auto a = in.template Get<A>();
  auto b = in.template Get<B>();
  auto weight = in.template Get<Weight>();

  return a * weight + b * (1 - weight);
}

int main() {
  std::cout << fun(FParams::Create().Set<A>(1.3f).Set<B>(2.4f).Set<Weight>(
                   0.1f))
            << std::endl;
}