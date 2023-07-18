#include <function.h>
#include <functional>
#include <iostream>

void func() { std::cout << "func" << std::endl; }

struct functor {
  void operator()() { std::cout << "functor" << std::endl; }
};

void func_1() { std::cout << "function" << std::endl; }

template <typename T> void print_size(T &t) {
  std::cout << sizeof(T) << ' ' << sizeof(t) << std::endl;
}

int main() {
  function_a<void()> f;
  f = func;
  f();
  print_size(f);
  f = functor();
  f();
  print_size(f);
  f = []() { std::cout << "lambda" << std::endl; };
  f();
  print_size(f);
  f = std::function<void()>(func_1);
  f();
  print_size(f);

  function_b<void()> g;
  g = func;
  g();
  print_size(g);
  g = functor();
  g();
  print_size(g);
  g = []() { std::cout << "lambda" << std::endl; };
  g();
  print_size(g);
  g = std::function<void()>(func_1);
  g();
  print_size(g);

  function_c<void()> h;
  h = func;
  h();
  print_size(h);
  h = functor();
  h();
  print_size(h);
  h = []() { std::cout << "lambda" << std::endl; };
  h();
  print_size(h);
  h = std::function<void()>(func_1);
  h();
  print_size(h);
}