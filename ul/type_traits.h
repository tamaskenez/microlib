#pragma once

#include <type_traits>
#include <vector>
#include <array>
#include <iterator>

#define UL_T_ENABLE_IF(X) typename = typename std::enable_if<X>::type

namespace ul {

// c_range_code >= 0 means fixed-size, indexable range
constexpr const ptrdiff_t c_range_code_indexable = -1;
constexpr const ptrdiff_t c_range_code_sized_iterable = -2;
constexpr const ptrdiff_t c_range_code_iterable = -3;
constexpr const ptrdiff_t c_range_code_unknown = -1000;

template <class T>
struct range_code : std::integral_constant<ptrdiff_t, c_range_code_unknown>
{
};

template <class T>
struct range_code<std::vector<T>>
    : std::integral_constant<ptrdiff_t, c_range_code_indexable>
{
};

template <class T, size_t N>
struct range_code<std::array<T, N>> : std::integral_constant<ptrdiff_t, N>
{
};

template <class SpanLike>
struct value_type
{
    using type = typename std::decay<decltype(std::begin(
        std::declval<typename std::add_const<SpanLike>::type>()))>::type;
};
}
