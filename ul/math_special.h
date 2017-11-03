#pragma once

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

#include "ul/math.h"
#include "ul/span.h"

// special mathematical functions
namespace ul {
// There are N groups with Ci choices (0 <= i < N). Select a choice from
// each group 0 <= ci < Ci. Enumerate all possibilites.
// The function `f` will be called with `span<I>` of length N, containing a
// possible set of choices.
// `I` is expected to be an integral type.
// Possibilities will be enumerated in lexicographic order.
// All items in Cs must be greater than zero.
template <class F, class I>
void choose_one_from_each_group(span<I> Cs, F&& f)
{
    if (Cs.empty())
        return;
    assert(std::all_of(Cs.begin(), Cs.end(), [](auto i) { return i > 0; }));
    std::vector<std::decay_t<I>> v(Cs.size(), I(0));
    span<I> span_out(v.data(), v.size());
    for (;;) {
        f(span_out);
        int i = v.size() - 1;
        while (++v[i] >= Cs[i]) {
            if (i == 0)
                return;
            v[i--] = I(0);
        }
    }
}

// Derivatives of tan x, deg 0..3
template <int N, class T>
T difftan(T x)
{
    if constexpr (N == 0)
        return std::tan(x);
    else if constexpr (N == 1)
        return square(ul::sec(x));
    else if constexpr (N == 2)
        return 2 * square(ul::sec(x)) * std::tan(x);
    else if constexpr (N == 3) {
        const double sx = ul::sec(x);
        return 4 * square(sx * std::tan(x)) + 2 * square(square(sx));
    }
    static_assert(0 <= N && N <= 3,
                  "tan derivatives implemented only up to 3.");
}

// Derivatives of cos x
template <int N, class T>
T diffcos(T x)
{
    if constexpr (N % 4 == 0)
        return std::cos(x);
    else if constexpr (N % 4 == 1)
        return -std::sin(x);
    else if constexpr (N % 4 == 2)
        return -std::cos(x);
    else if constexpr (N % 4 == 3)
        return std::sin(x);
    std::terminate();
}

// Derivatives of sin x
template <int N, class T>
T diffsin(T x)
{
    if constexpr (N % 4 == 0)
        return std::sin(x);
    else if constexpr (N % 4 == 1)
        return std::cos(x);
    else if constexpr (N % 4 == 2)
        return -std::sin(x);
    else if constexpr (N % 4 == 3)
        return -std::cos(x);
    std::terminate();
}

// Taylor series of tan(x) around `a`. Return array with coefficients of the
// polynomial. Deg = 0..3
template <int Deg, class T>
std::array<T, Deg + 1> taylor_tan(T a)
{
    static_assert(Deg >= 0);
    std::array<T, Deg + 1> v;
    v[0] = tan(a);
    if constexpr (Deg >= 1) {
        v[1] = difftan<1>(a);
        v[0] -= a * v[1];
        if constexpr (Deg >= 2) {
            v[2] = difftan<2>(a) / 2;
            v[1] -= 2 * a * v[2];
            v[0] += a * a * v[2];
            if constexpr (Deg >= 3) {
                static_assert(Deg == 3,
                              "Taylor series implemented only up to 3.");
                v[3] = difftan<3>(a) / 6;
                v[2] -= 3 * a * v[3];
                v[1] += 3 * a * a * v[3];
                v[0] -= a * a * a * v[3];
            }
        }
    }
    return v;
}

// Taylor series of cos(x) around `a`. Return array with coefficients of the
// polynomial. Deg = 0..3
template <int Deg, class T>
std::array<T, Deg + 1> taylor_cos(T a)
{
    static_assert(Deg >= 0);
    std::array<T, Deg + 1> v;
    v[0] = cos(a);
    if constexpr (Deg >= 1) {
        v[1] = diffcos<1>(a);
        v[0] -= a * v[1];
        if constexpr (Deg >= 2) {
            v[2] = diffcos<2>(a) / 2;
            v[1] -= 2 * a * v[2];
            v[0] += a * a * v[2];
            if constexpr (Deg >= 3) {
                static_assert(Deg == 3,
                              "Taylor series implemented only up to 3.");
                v[3] = diffcos<3>(a) / 6;
                v[2] -= 3 * a * v[3];
                v[1] += 3 * a * a * v[3];
                v[0] -= a * a * a * v[3];
            }
        }
    }
    return v;
}

template <int Deg, class T>
std::array<T, Deg + 1> taylor_sin(T a)
{
    static_assert(Deg >= 0);
    std::array<T, Deg + 1> v;
    v[0] = sin(a);
    if constexpr (Deg >= 1) {
        v[1] = diffsin<1>(a);
        v[0] -= a * v[1];
        if constexpr (Deg >= 2) {
            v[2] = diffsin<2>(a) / 2;
            v[1] -= 2 * a * v[2];
            v[0] += a * a * v[2];
            if constexpr (Deg >= 3) {
                static_assert(Deg == 3,
                              "Taylor series implemented only up to 3.");
                v[3] = diffsin<3>(a) / 6;
                v[2] -= 3 * a * v[3];
                v[1] += 3 * a * a * v[3];
                v[0] -= a * a * a * v[3];
            }
        }
    }
    return v;
}

}  // namespace ul
