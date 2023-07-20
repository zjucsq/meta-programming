//
// Created by csq on 2/7/23.
//
#include "chapter1.h"
#include "chapter2.h"
#include <deque>
#include <iostream>
#include <list>
#include <tuple.h>
#include <tuple>
#include <type_traits>
#include <vector>

int main() {
  std::cout << Size<std::tuple<int, double, float>> << std::endl;
  std::cout << Size<std::list<int>> << std::endl;
  std::cout << Size<std::vector<int>> << std::endl;
  std::cout << Size<std::deque<int>> << std::endl;
  std::cout << Size<int> << std::endl;
  std::cout << Accumulate<1, 2, 3, 4, 5> << std::endl;

  static_assert(
      std::is_same_v<std::tuple<int *, float *>, std::tuple<int *, float *>>);
  static_assert(std::is_same_v<int *, std::add_pointer_t<int>>);
  static_assert(std::is_same_v<
                std::tuple<int *, float *>,
                Transform<tuple<int, float>, std::add_pointer, std::tuple>>);
}
