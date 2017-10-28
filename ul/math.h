#pragma once

#include <cassert>
#include <cmath>

namespace ul {
template <class X, class L, class U>
bool within_co(const X& x, const L& lower, const U& upper)
{
    assert(lower <= upper);
    return lower <= x && x < upper;
}

template <class T>
constexpr int sign(T x) noexcept
{
    return x == T(0) ? 0 : (std::signbit(x) ? -1 : 1);
}

}  // namespace ul
