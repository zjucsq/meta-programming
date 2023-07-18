//
// Created by csq on 5/11/23.
//
#include <memory>

class XXX : public std::enable_shared_from_this<XXX> {
public:
  std::shared_ptr<XXX> f() { return shared_from_this(); }
};

int main() {
  //    XXX* px0 = new XXX{};
  //    px0->f(); // Ops! Throw std::bad_weak_ptr

  XXX *px = new XXX{};
  std::shared_ptr<XXX> spx(px);
  spx->f();
}