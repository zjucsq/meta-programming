//
// Created by csq on 5/13/23.
//
// by chatgpt
#include <iostream>
#include <utility>

template <typename T1, typename T2>
struct pair {
    T1 first;
    T2 second;

    pair() = default;
    pair(const T1 &t1, const T2 &t2) : first(t1), second(t2) {}
    template <typename U1, typename U2>
    pair(U1 &&u1, U2 &&u2) : first(std::forward<U1>(u1)), second(std::forward<U2>(u2)) {}

    template <typename U1, typename U2>
    pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}

    template <typename U1, typename U2>
    pair(pair<U1, U2> &&other) : first(std::forward<U1>(other.first)), second(std::forward<U2>(other.second)) {}

    pair &operator=(const pair &other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    pair &operator=(pair &&other) {
        if (this != &other) {
            first = std::forward<T1>(other.first);
            second = std::forward<T2>(other.second);
        }
        return *this;
    }

    void swap(pair &other) {
        using std::swap;
        swap(first, other.first);
        swap(second, other.second);
    }

    bool operator==(const pair &rhs) const { return first == rhs.first && second == rhs.second; }

    bool operator!=(const pair &rhs) const { return !(*this == rhs); }

    bool operator<(const pair &rhs) const { return first < rhs.first || (!(rhs.first < first) && second < rhs.second); }

    bool operator<=(const pair &rhs) const { return !(rhs < *this); }

    bool operator>(const pair &rhs) const { return rhs < *this; }

    bool operator>=(const pair &rhs) const { return !(*this < rhs); }
};
