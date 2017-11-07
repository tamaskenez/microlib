#pragma once

#include "ul/span.h"

#include <type_traits>
#include <utility>

#include "ul/check.h"
#include "ul/inlinevector.h"
#include "ul/math.h"
#include "ul/size_bounds.h"
#include "ul/type_traits.h"

// matlab-like functions

namespace ul {

template <class V, class U>
auto polyval(const V& p, U x) -> decltype(p[0] * U(0))
{
    using R = decltype(p[0] * U(0));
    switch (p.size()) {
        case 0:
            return 0;
        case 1:
            return p[0];
        case 2:
            return p[0] + x * p[1];
        default:
            R xa = x;
            R acc = p[0] + p[1] * x;
            for (int i = 2; i < p.size(); ++i) {
                xa *= x;
                acc += p[i] * xa;
            }
            return acc;
    }
}

template <class V>
auto polyder(const V& p)
{
#if 0
    using traits = sequence_compile_time_size_traits<V>;
    static_assert(traits::capacity == c_runtime_size_marker ||
                  traits::capacity > 0);
    static_assert(traits::size == c_runtime_size_marker || traits::size > 0);
    UL_CHECK(p.size() > 0);
    constexpr size_t N = traits::capacity == c_runtime_size_marker
                             ? c_runtime_size_marker
                             : traits::capacity - 1;
    using T = UL_DECAYDECL(p[0]);
    auto result = make_uninitialized_array_or_inlinevector_or_vector<
        T, N, traits::size != c_runtime_size_marker>(p.size() - 1);
#else
    auto p_size_bounds = get_size_bounds(p);
    static_assert(
        p_size_bounds.compile_time_capacity == c_runtime_size_marker ||
            p_size_bounds.compile_time_capacity > 0,
        "polyder: argument has zero size.");
    assert(p.size() > 0);
    auto result =
        make_uninitialized_array_or_inlinevector_or_vector<UL_DECAYDECL(p[0])>(
            p_size_bounds - size_bounds_constant<1>());
#endif
    assert(result.size() + 1 == p.size());
    for (int i = 0; i + 1 < p.size(); ++i) {
        result[i] = (i + 1) * p[i + 1];
    }
    return result;
}

template <class V>
auto polyint(const V& p, UL_DECAYDECL(p[0]) C0 = 0)
{
#if 0
    using traits = sequence_compile_time_size_traits<V>;
    constexpr size_t N = traits::capacity == c_runtime_size_marker
                             ? c_runtime_size_marker
                             : traits::capacity + 1;
    using T = UL_DECAYDECL(p[0]);
    auto result = make_uninitialized_array_or_inlinevector_or_vector<
        T, N, traits::size != c_runtime_size_marker>(p.size() + 1);
#else
    auto p_size_bounds = get_size_bounds(p);
    static_assert(
        p_size_bounds.compile_time_capacity == c_runtime_size_marker ||
            p_size_bounds.compile_time_capacity > 0,
        "polyint: argument has zero size.");
    assert(p.size() > 0);
    auto result =
        make_uninitialized_array_or_inlinevector_or_vector<UL_DECAYDECL(p[0])>(
            p_size_bounds + size_bounds_constant<1>());
#endif
    assert(result.size() == p.size() + 1);
    result[0] = C0;
    for (int i = 0; i < p.size(); ++i) {
        result[i + 1] = p[i] / (i + 1);
    }
    return result;
}

// This is the previous implementation of conv, will be removed if the
// size_bounds-based proves stable.

template <class X, class Y, class Result>
void conv_into_nocheck(const X& x, const Y& y, Result&& result)
{
    // result is initialized to zero
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < y.size(); ++j) {
            result[i + j] += x[i] * y[j];
        }
    }
}

template <class X, class Y>
auto conv_get_result_size_bounds(const X& x, const Y& y)
{
    auto x_size_bounds = get_size_bounds(x);
    auto y_size_bounds = get_size_bounds(y);

    static_assert(
        x_size_bounds.compile_time_capacity == c_runtime_size_marker ||
            x_size_bounds.compile_time_capacity > 0,
        "conv: first argument has zero size.");
    static_assert(
        y_size_bounds.compile_time_capacity == c_runtime_size_marker ||
            y_size_bounds.compile_time_capacity > 0,
        "conv: first argument has zero size.");

    return max(x_size_bounds + y_size_bounds - size_bounds_constant<1>(),
               size_bounds_constant<0>());
}

template <class X, class Y>
auto conv(const X& x, const Y& y)
{
    auto result =
        make_zero_initialized_array_or_inlinevector_or_vector<decltype(
            x[0] * y[0])>(conv_get_result_size_bounds(x, y));

    conv_into_nocheck(x, y, result);

    return result;
}

template <class X, class Y, class Result>
void conv_into(const X& x, const Y& y, Result&& result)
{
    auto r_size_bounds_expected = conv_get_result_size_bounds(x, y);

    if constexpr (is_resizable<std::decay_t<Result>>::value) {
        // if resizable, resize
        const auto target_size = r_size_bounds_expected.runtime_size();

        // zero at most until current result size
        std::fill(result.begin(),
                  result.begin() + std::min<int>(target_size, result.size()),
                  0);

        // resize and zero the rest
        result.resize(target_size, 0);
    } else {
        // if not resizable, check size ...
        auto r_size_bounds_actual = get_size_bounds(result);
        if constexpr (r_size_bounds_actual.compile_time_size ==
                          c_runtime_size_marker ||
                      r_size_bounds_expected.compile_time_size ==
                          c_runtime_size_marker) {
            // ... at runtime ...
            CHECK(result.size() == r_size_bounds_expected.runtime_size());
        } else {
            // .. or at compile time.
            static_assert(r_size_bounds_actual.compile_time_size ==
                          r_size_bounds_expected.compile_time_size);
        }
        std::fill(BE(result), 0);
    }
    conv_into_nocheck(x, y, result);
}

template <class T>
auto rad2deg(T x)
{
    return x / M_PI * 180;
}

template <class T>
auto deg2rad(T x)
{
    return x / 180.0 * M_PI;
}

}  // namespace ul
