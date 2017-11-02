#pragma once

#include "ul/span.h"

#include <type_traits>
#include <utility>

#include "ul/check.h"
#include "ul/inlinevector.h"
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

    for (int i = 0; i + 1 < p.size(); ++i) {
        result[i] = (i + 1) * p[i + 1];
    }
    return result;
}

template <class V>
auto polyint(const V& p, UL_DECAYDECL(p[0]) C0 = 0)
{
    using traits = sequence_compile_time_size_traits<V>;
    constexpr size_t N = traits::capacity == c_runtime_size_marker
                             ? c_runtime_size_marker
                             : traits::capacity + 1;
    using T = UL_DECAYDECL(p[0]);
    auto result = make_uninitialized_array_or_inlinevector_or_vector<
        T, N, traits::size != c_runtime_size_marker>(p.size() + 1);

    result[0] = C0;
    for (int i = 0; i < p.size(); ++i) {
        result[i + 1] = p[i] / (i + 1);
    }
    return result;
}

constexpr size_t conv_result_size(size_t x, size_t y)
{
    return (x == 0 || y == 0) ? 0 : x + y - 1;
}

template <class X, class Y>
auto conv(const X& x, const Y& y)
{
    // We need a result array or InlineVector or vector depending on the
    // compile-time size characteristics of the inputs
    using r_value_t = decltype(x[0] * y[0]);
    using x_traits = sequence_compile_time_size_traits<X>;
    using y_traits = sequence_compile_time_size_traits<Y>;

    constexpr bool has_compile_time_capacity =
        x_traits::capacity != c_runtime_size_marker &&
        y_traits::capacity != c_runtime_size_marker;
    constexpr bool has_compile_time_size =
        x_traits::size != c_runtime_size_marker &&
        y_traits::size != c_runtime_size_marker;

    constexpr size_t r_compile_time_capacity_or_size_or_marker =
        has_compile_time_capacity
            ? conv_result_size(x_traits::capacity, y_traits::capacity)
            : c_runtime_size_marker;

#if 0
    typename ul::array_or_inlinevector_or_vector<
        r_value_t, r_compile_time_capacity_or_size_or_marker, has_compile_time_capacity,
        has_compile_time_size>::type result{};  // zero-initializes array

    if constexpr (!has_compile_time_size)
        result.resize(conv_result_size(x.size(), y.size()));
#else
    auto result = make_zero_initialized_array_or_inlinevector_or_vector<
        r_value_t, r_compile_time_capacity_or_size_or_marker,
        has_compile_time_size>(conv_result_size(x.size(), y.size()));
#endif
    // result is initialized to zero

    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < y.size(); ++j) {
            result[i + j] += x[i] * y[j];
        }
    }
    return result;
}

}  // namespace ul
