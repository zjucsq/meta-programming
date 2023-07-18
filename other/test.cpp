//
// Created by csq on 2/7/23.
//
#include <iostream>

using namespace std;

template <class T1, class T2> class Test {
public:
  Test(T1 a, T2 b) : _a(a), _b(b) { cout << "模板化" << endl; }

private:
  T1 _a;
  T2 _b;
};

// 模板全特化
template <> class Test<int, int> {
public:
  Test(int a, int b) : _a(a), _b(b) { cout << "模板全特化" << endl; }

private:
  int _a;
  int _b;
};

// 模板偏特化
template <class T> class Test<int, T> {
public:
  Test(int a, T b) : _a(a), _b(b) { cout << "模板偏特化" << endl; }

private:
  int _a;
  T _b;
};

int main() {
  Test<double, double> t1(1.01, 1.01);
  Test<int, int> t2(1, 1);
  Test<int, char *> t3(1, "111");
  return 0;
}