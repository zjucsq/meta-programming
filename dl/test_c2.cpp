//
// Created by csq on 2/7/23.
//
#include "chapter1.h"
#include "chapter2.h"
#include <deque>
#include <iostream>
#include <list>
#include <vector>

int main() {
  std::cout << Size<std::tuple<int, double, float>> << std::endl;
  std::cout << Size<std::list<int>> << std::endl;
  std::cout << Size<std::vector<int>> << std::endl;
  std::cout << Size<std::deque<int>> << std::endl;
  std::cout << Size<int> << std::endl;
  std::cout << Accumulate<1, 2, 3, 4, 5> << std::endl;
}
