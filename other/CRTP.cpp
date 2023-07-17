//
// Created by csq on 5/11/23.
//
#include <iostream>

template <typename T>
struct B {
    void call() { static_cast<T *>(this)->run(); }
};

struct D1 : public B<D1> {
    void run() { std::cout << "D1" << std::endl; }
};

struct D2 : public B<D2> {
    void run() { std::cout << "D2" << std::endl; }
};

int main() {
    D1 d1;
    D2 d2;
    B<D1> *b1 = &d1;
    B<D2> *b2 = &d2;
    d1.call();
    d2.call();
    b1->call();
    b2->call();
}
