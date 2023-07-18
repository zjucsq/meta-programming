//
// Created by csq on 5/13/23.
//
#include <iostream>
#include <string>
#include <typeinfo>

class vartype {
public:
  vartype() = delete;

  template <typename T> vartype(const T &value) : type_(typeid(value)) {
    value_ = static_cast<void *>(new T{value});
  }

  vartype(const vartype &other) {
    value_ = static_cast<void *>(new decltype(other.value_){other.value_});
  }

  vartype &operator=(const vartype &other) {
    if (this != &other) {
      if (value_)
        delete value_;
      m_type = other.m_type;
    }
    return *this;
  }

  ~vartype() { delete value_; }

  template <typename T> T &get() {
    if (typeid(T) == type_) {
      return *static_cast<T *>(value_);
    } else {
      throw std::bad_cast();
    }
  }

  const std::type_info &type_info() const { return type_; }

private:
  void *value_;
  const std::type_info &type_;
};

int main() {
  vartype a = 123;
  vartype b = std::string("hello");
  // vartype c = "hello";   wrong

  try {
    int x = a.get<int>();
    std::cout << "a: " << x << std::endl;
    std::string s = b.get<std::string>();
    std::cout << "b: " << s << std::endl;
  } catch (std::bad_cast &e) {
    std::cerr << "类型不匹配" << std::endl;
  }

  return 0;
}
