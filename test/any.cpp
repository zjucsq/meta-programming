#include <any.h>

int main() {
    any a = 123;
    any b = std::string("hello");

    std::cout << a.any_cast<int>() << std::endl;
    std::cout << b.any_cast<std::string>() << std::endl;

    any c;
    std::cout << a.has_value() << b.has_value() << c.has_value() << std::endl;
    c = std::move(a);
    std::cout << a.has_value() << b.has_value() << c.has_value() << std::endl;

    return 0;
}
