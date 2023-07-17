# 

Default template parameters
```C++
template <class T, size_t N = 10>
struct my_array {
    T arr[N];
};
int main() {
    my_array<int, 5> a;
    std::cout << sizeof(a.arr) / sizeof(int) << std::endl;
    my_array<int> b;
    std::cout << sizeof(b.arr) / sizeof(int) << std::endl;
}
// output: 5 10

template <class T, size_t N = 10>
struct my_array {
    T arr[N];
};
template <class T>
struct my_array<T> {
    T arr[2];
};
int main() {
    my_array<int, 5> a;
    std::cout << sizeof(a.arr) / sizeof(int) << std::endl;
    my_array<int> b;
    std::cout << sizeof(b.arr) / sizeof(int) << std::endl;
}
// output: 5 2 偏特化模板等于my_array<T, 10>，所以b匹配但是a不匹配

template <class T, size_t N = 10>
struct my_array {
    T arr[N];
};
template <class T>
struct my_array<T, 5> {
    T arr[2];
};
int main() {
    my_array<int, 5> a;
    std::cout << sizeof(a.arr) / sizeof(int) << std::endl;
    my_array<int> b;
    std::cout << sizeof(b.arr) / sizeof(int) << std::endl;
}
// output: 2 10 默认参数10不能匹配特化模板，但是指定的参数5可以

template <class T, size_t N = 10>
struct my_array {
    T arr[N];
};
template <class T>
struct my_array<T, 6> {
    T arr[2];
};
int main() {
    my_array<int, 5> a;
    std::cout << sizeof(a.arr) / sizeof(int) << std::endl;
    my_array<int> b;
    std::cout << sizeof(b.arr) / sizeof(int) << std::endl;
}
// output: 5 10 默认参数10和指定的参数5都不能匹配特化模板
```