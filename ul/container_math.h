#pragma once

#include <array>
#include <vector>

#include "ul/check.h"

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
auto operator/(const std::array<X, N>& x, const Y& y)
{
    std::array<decltype(x[0] / y), N> r;
    for (int i = 0; i < N; ++i) {
        r[i] = x[i] / y;
    }
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

}  // namespace vector_math
}  // namespace ul
