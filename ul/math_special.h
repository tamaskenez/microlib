#pragma once

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

#include "ul/math.h"
#include "ul/size_bounds.h"
#include "ul/span.h"
#include "ul/ul.h"

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

// p and q are two functions described as polynomials. This function creates the
// polynomial for p(q(x)).
template <class P, class Q>
auto polycompose(const P& p, const Q& q)
{
    auto p_bounds = get_size_bounds(p);
    auto q_bounds = get_size_bounds(q);
    static_assert(p_bounds.compile_time_capacity > 0 &&
                      q_bounds.compile_time_capacity > 0,
                  "polycompose: both input arguments must be non-empty.");
    CHECK(p_bounds.runtime_size() > 0 && q_bounds.runtime_size() > 0,
          "polycompose: both input arguments must be non-empty.");
    size_bounds_constant<1> sb_one;
    size_bounds_constant<2> sb_two;
    auto r_bounds =
        (p_bounds - sb_one) * (q_bounds - sb_one) +
        sb_one;  // results' degree = product of degrees of the inputs
    using r_value_t = decltype(p[0] * q[0]);
    auto result =
        make_zero_initialized_array_or_inlinevector_or_vector<r_value_t>(
            r_bounds);

    // Now we calculate p(y) where y = q(x). The sum p(y) consists of `coeff *
    // y^i` terms. First i = 0 -> result += p[0] * q^0 = p[0]
    result[0] = p[0];  // Using the fact that it's zero-initialized

    // Do we need more than q^0?
    if constexpr (p_bounds.compile_time_size != c_runtime_size_marker) {
        if constexpr (p_bounds.compile_time_size <= 1)
            return result;
    } else {
        // Need to check it in runtime
        if (p.size() <= 1)
            return result;
    }

    // Next, i = 1 -> result += x * p[1] * q^1 = x * p[1] * q
    FOR(j, 0, < q.size()) { result[j] += p[1] * q[j]; }

    // Do we need more than q^1?
    if constexpr (p_bounds.compile_time_size != c_runtime_size_marker) {
        if constexpr (p_bounds.compile_time_size <= 2)
            return result;
    } else {
        // Need to check it in runtime
        if (p.size() <= 2)
            return result;
    }

    // And all the rest: result += x^i * p[i] * q^i
    // First create q^2
    auto q_ad_i =
        make_zero_initialized_array_or_inlinevector_or_vector<r_value_t>(
            r_bounds);
    FOR(ja, 0, < q.size())
    {
        q_ad_i[2 * ja] += q[ja] * q[ja];
        FOR(jb, ja + 1, < q.size())
        q_ad_i[ja + jb] += 2 * q[ja] * q[jb];
    }
    int q_ad_i_highest_nonzero_ix = 2 * (q.size() - 1);
    FOR(j, 0, <= q_ad_i_highest_nonzero_ix) { result[j] += p[2] * q_ad_i[j]; }

    // Do we need more than q^2?
    if constexpr (p_bounds.compile_time_size != c_runtime_size_marker) {
        if constexpr (p_bounds.compile_time_size <=
                      3)  // size == 3 -> degs == 2
            return result;
    } else {
        // Need to check it in runtime
        if (p.size() <= 3)
            return result;
    }
    if constexpr (p_bounds.compile_time_size == c_runtime_size_marker ||
                  p_bounds.compile_time_size > 3) {
        // We need a temporary intermediate storage for the `q_ad_i =
        // conv(q_ad_i, q)` operation. It must hold at most q^(i-1)
        auto q_ad_i_copy_bounds =
            (q_bounds - sb_one) * (p_bounds - sb_two) + sb_one;

        auto q_ad_i_copy =
            make_uninitialized_array_or_inlinevector_or_vector<r_value_t>(
                q_ad_i_copy_bounds);

        // Enter loop for i at 2
        for (int i = 3; i < p.size(); ++i) {
            // On first iteration we have q^2 in q_ad_i

            // Copy q_ad_i to temp storage.
            FOR(j, 0, <= q_ad_i_highest_nonzero_ix)
            {
                q_ad_i_copy[j] = q_ad_i[j];
            }

            auto q_ad_i_copy_span =
                make_span(q_ad_i_copy.data(), q_ad_i_highest_nonzero_ix + 1);
            q_ad_i_highest_nonzero_ix = i * (q.size() - 1);

            // q_ad_i = conv(q_ad_i, q)
            conv_into(q_ad_i_copy_span, q,
                      make_span(q_ad_i.data(), q_ad_i_highest_nonzero_ix + 1));

            // result += x^i * p[i] * q_ad_i;
            FOR(j, 0, <= q_ad_i_highest_nonzero_ix)
            {
                result[j] += p[i] * q_ad_i[j];
            }
        }
        return result;
    }
}

}  // namespace ul
