#pragma once

#include <array>
#include <bitset>
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

template <class T>
struct is_fixed_size_range : std::false_type
{
};

template <class U, std::size_t N>
struct is_fixed_size_range<std::array<U, N>> : std::true_type
{
};

template <class T, size_t N, bool NeedArray>
struct array_or_vector;

template <class T, size_t N>
struct array_or_vector<T, N, false>
{
    using type = std::vector<T>;
};

template <class T, size_t N>
struct array_or_vector<T, N, true>
{
    using type = std::array<T, N>;
};

template <class T, size_t N, bool NeedArray>
constexpr typename array_or_vector<T, N, NeedArray>::type
make_array_or_vector_of_given_size(size_t n)
{
    if constexpr (NeedArray)
        return std::array<T, N>{};
    else
        return std::vector<T>(n);
}

template <class T>
struct size_of_fixed_size_range_or_zero
{
    static constexpr size_t value = 0;
};

template <class T, size_t N>
struct size_of_fixed_size_range_or_zero<std::array<T, N>>
{
    static constexpr size_t value = N;
};

}  // namespace ul
