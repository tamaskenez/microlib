#pragma once

// Simple mathematical functions that could be part of standard library
#include <cassert>
#include <cmath>
#include <vector>

#include "ul/span.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

namespace ul {

template <class X, class L, class U>
bool within_co(const X& x, const L& lower, const U& upper)
{
    assert(lower <= upper);
    return lower <= x && x < upper;
}

template <class T>
bool is_zero(T x)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif
    return x == 0;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

template <class T>
constexpr int sign(T x) noexcept
{
    return is_zero(x) ? 0 : (std::signbit(x) ? -1 : 1);
}

template <class T>
T square(T x)
{
    return x * x;
}

template <class T>
T sec(T x)  // secant
{
    return 1 / cos(x);
}

}  // namespace ul
