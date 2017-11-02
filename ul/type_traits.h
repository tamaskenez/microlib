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

// type_traits-style template-meta-horror for creating polymorph functions
// which could take and return views and containers of various compile-time
// / runtime size characteristics.
//
// That is, a function, depending on whether is input sequences (ranges,
// views, containers, whatever) have compile-time or runtime capacity or
// size could return an array, InlineVector or vector.

// First we need to know if a particular sequence type has runtime or
// compile-time capacity and size. This trait type is to be extended
// with new types as they are introduced, for example, we could have
// a fixed-compile-time sized span in the future.
// For now:
//
// - std::array has compile-time capacity and size
// - ul::InlineVector has compile-time capacity but runtime size
// - std::vector: all runtime

// unique value meaning i-don't-know-this-in-compile-time
static constexpr size_t c_runtime_size_marker = ~size_t(0);

// traits giving compile-time capacity and size values. For InlineVector it
// is implemented in its header.
template <class T>
struct sequence_compile_time_size_traits
{
    static constexpr size_t capacity = c_runtime_size_marker;
    static constexpr size_t size = c_runtime_size_marker;
};

template <class T, size_t N>
struct sequence_compile_time_size_traits<std::array<T, N>>
{
    static constexpr size_t capacity = N;
    static constexpr size_t size = N;
};

// The next type yields array or InlineVector or vector depending on the
// HasCompileTimeCapacity and HasCompileTimeSize boolean template
// parameters. For InlineVector see its header.

// Rules:
// - HasCompileTimeCapacity && HasCompileTimeSize => std::array<T, N>
// - HasCompileTimeCapacity && !HasCompileTimeSize => ul::InlineVector<T, N>
// - !HasCompileTimeCapacity && !HasCompileTimeSize => std::vector<T>, N is
//   ignored
// The fourth combination is invalid.
template <class T,
          size_t N,
          bool HasCompileTimeCapacity,
          bool HasCompileTimeSize>
struct array_or_inlinevector_or_vector;

template <class T, size_t N>
struct array_or_inlinevector_or_vector<T, N, false, false>
{
    using type = std::vector<T>;
};

template <class T, size_t N>
struct array_or_inlinevector_or_vector<T, N, true, true>
{
    using type = std::array<T, N>;
};

// see bottom of inlinevector.h for the make_*_array_inlinevector_or_vector
// helper function

}  // namespace ul
