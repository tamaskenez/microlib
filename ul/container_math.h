#pragma once

#include <array>
#include <cmath>
#include <vector>

#include "ul/check.h"
#include "ul/ul.h"

namespace ul {
namespace array_math {

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

}  // namespace vector_math
}  // namespace ul
