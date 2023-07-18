#include <pair.h>

int main() {
  pair<int, double> p{5, 4.5};
  std::cout << p.first << ' ' << p.second << std::endl;
}