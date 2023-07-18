#include <tuple.h>

int main() {
  tuple<int, int, std::string> t(114, 514, "1919810");
  //    std::cout << t.get() << std::endl;
  std::cout << get<0>(t) << std::endl;
  std::cout << get<1>(t) << std::endl;
  std::cout << get<2>(t) << std::endl;
}
