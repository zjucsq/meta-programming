#include <variant>

int main() {
  variant<int, double, std::string> v1 = 42;
  variant<int, double, std::string> v2 = 3.14;
  variant<int, double, std::string> v3 = std::string("hello");

  std::cout << v1.get<int>() << '\n';
  std::cout << v2.get<double>() << '\n';
  std::cout << v3.get<std::string>() << '\n';

  std::cout << v1.index() << '\n';
  std::cout << v2.index() << '\n';
  std::cout << v3.index() << '\n';

  std::cout << v1.get<0>() << '\n';
  std::cout << v2.get<1>() << '\n';
  std::cout << v3.get<2>() << '\n';

  v1 = v2;

  std::cout << v1.get<double>() << '\n';

  return 0;
}
