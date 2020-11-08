#pragma once

#include <array>
#include <cmath>
#include <numeric>
#include <vector>

#include "ul/check.h"
#include "ul/ul.h"

namespace ul {
namespace array_math {

template <class X, class Y, size_t N>
auto operator+(const std::array<X, N>& x, Y y)
{
    std::array<decltype(x[0] + y), N> r;
    for (int i = 0; i < N; ++i) {
        r[i] = x[i] + y;
    }
    return r;
}

template <class X, class Y, size_t N>
auto operator-(const std::array<X, N>& x, const std::array<Y, N>& y)
{
    std::array<decltype(x[0] - y[0]), N> r;
    for (int i = 0; i < N; ++i) {
        r[i] = x[i] - y[i];
    }
    return r;
}

template <class X, class Y, size_t N>
auto operator/(const std::array<X, N>& x, const Y y)
{
    std::array<decltype(x[0] / y), N> z;
    FOR(i, 0, < N) { z[i] = x[i] / y; }
    return z;
}

template <class X, class Y, size_t N>
auto operator/(X x, const std::array<Y, N>& y)
{
    std::array<decltype(x / y[0]), N> z;
    FOR(i, 0, < N) { z[i] = x / y[i]; }
    return z;
}

template <class X, class Y, size_t N>
std::array<X, N>& operator/=(std::array<X, N>& x, const Y& y)
{
    FOR(i, 0, < N) { x[i] /= y; }
    return x;
}

template <class X, size_t N>
auto sum(const std::array<X, N>& x)
{
    X s(0);
    FOR(i, 0, < N) { s += x[i]; }
    return s;
}

template <class X, size_t N>
auto log(const std::array<X, N>& x)
{
    std::array<X, N> r;
    FOR(i, 0, < N) { r[i] = std::log(x[i]); }
    return r;
}

template <class X, class Y, size_t N>
auto times(const std::array<X, N>& x, const std::array<Y, N>& y)
{
    std::array<decltype(x[0] * y[0]), N> r;
    FOR(i, 0, < N) { r[i] = x[i] * y[i]; }
    return r;
}

template <size_t N>
bool all_true(const std::array<bool, N>& bs)
{
    for (auto b : bs) {
        if (!b) {
            return false;
        }
    }
    return true;
}

template <size_t N>
bool any_true(const std::array<bool, N>& bs)
{
    for (auto b : bs) {
        if (b) {
            return true;
        }
    }
    return false;
}

template <class X, class Y, size_t N>
auto min_elementwise(const std::array<X, N>& x, const std::array<Y, N>& y)
{
    using Z = decltype(x[0] + y[0]);
    std::array<Z, N> z;
    FOR(i, 0, < N) { z[i] = std::min<Z>(x[i], y[i]); }
    return z;
}

template <class X, class Y, size_t N>
auto max_elementwise(const std::array<X, N>& x, const std::array<Y, N>& y)
{
    using Z = decltype(x[0] + y[0]);
    std::array<Z, N> z;
    FOR(i, 0, < N) { z[i] = std::max<Z>(x[i], y[i]); }
    return z;
}

}  // namespace array_math

namespace vector_math {
template <class X, class Y>
auto operator-(const std::vector<X>& x, const std::vector<Y>& y)
{
    std::vector<decltype(x[0] - y[0])> r;
    const auto N = x.size();
    CHECK(y.size() == N);
    r.reserve(N);
    for (int i = 0; i < N; ++i) {
        r.emplace_back(x[i] - y[i]);
    }
    return r;
}

template <class X, class Y>
void operator-=(std::vector<X>& x, const std::vector<Y>& y)
{
    const auto N = x.size();
    CHECK(y.size() == N);
    for (int i = 0; i < N; ++i) {
        x[i] -= y[i];
    }
}

template <class X, class Y>
auto operator*(const std::vector<X>& x, const Y& y)
{
    std::vector<decltype(x[0] * y)> r;
    const auto N = x.size();
    r.reserve(N);
    for (int i = 0; i < N; ++i) {
        r.emplace_back(x[i] * y);
    }
    return r;
}

template <class X, class Y>
auto operator*(const X& x, const std::vector<Y>& y)
{
    return y * x;
}

template <class X, class Y>
std::vector<X>& operator/=(std::vector<X>& x, const Y& y)
{
    FOR(i, 0, < x.size()) { x[i] /= y; }
    return x;
}

template <class X>
auto sum(const std::vector<X>& x)
{
    X s(0);
    for (auto i : x) {
        s += i;
    }
    return s;
}

template <class T>
auto iota_v(T start_value, int size)
{
    std::vector<T> v(size);
    std::iota(BE(v), start_value);
    return v;
}

template <class T = int>
auto iota_v(int size)
{
    return iota_v((T)0, size);
}

template <class X, class Y>
auto times(const std::vector<X>& x, const std::vector<Y>& y)
{
    const auto N = x.size();
    CHECK(y.size() == N);
    std::vector<decltype(x[0] * y[0])> r;
    r.reserve(N);
    FOR(i, 0, < N) { r.push_back(x[i] * y[i]); }
    return r;
}

}  // namespace vector_math
}  // namespace ul
