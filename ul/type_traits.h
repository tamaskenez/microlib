#pragma once

#include <array>
#include <bitset>
#include <climits>
#include <iterator>
#include <type_traits>
#include <vector>

#define UL_T_ENABLE_IF(X) typename = typename std::enable_if<X>::type
#define UL_DECAYDECL(EXPR) typename std::decay<decltype(EXPR)>::type

namespace ul {

// c_range_code >= 0 means fixed-size, indexable range
constexpr const ptrdiff_t c_range_code_indexable = -1;
constexpr const ptrdiff_t c_range_code_sized_iterable = -2;
constexpr const ptrdiff_t c_range_code_iterable = -3;
constexpr const ptrdiff_t c_range_code_unknown = -1000;

struct uninitialized_t
{};
constexpr const uninitialized_t uninitialized;

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
struct range_code<std::array<T, N>>
    : std::integral_constant<ptrdiff_t, (ptrdiff_t)N>
{
};

template <class T>
struct value_type
{
    using T1 =
        decltype(*std::begin(std::declval<typename std::add_const<T>::type>()));
    using type = typename std::decay<decltype(
        *std::begin(std::declval<typename std::add_const<T>::type>()))>::type;
};

}  // namespace ul
