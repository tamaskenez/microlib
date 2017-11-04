#pragma once

#include "ul/inlinevector.h"

namespace ul {

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
static constexpr size_t c_runtime_size_marker = INT_MAX;

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

template <class T, size_t N>
struct sequence_compile_time_size_traits<InlineVector<T, N>>
{
    static constexpr size_t capacity = N;
    static constexpr size_t size = c_runtime_size_marker;
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

template <class T, size_t N>
struct array_or_inlinevector_or_vector<T, N, true, false>
{
    using type = ul::InlineVector<T, N>;
};

template <class T, size_t CompileTimeCapacityOrSize, bool HasCompileTimeSize>
constexpr auto make_zero_initialized_array_or_inlinevector_or_vector(
    size_t runtime_size)
{
    if constexpr (CompileTimeCapacityOrSize != c_runtime_size_marker) {
        if constexpr (HasCompileTimeSize)
            return std::array<T, CompileTimeCapacityOrSize>{};
        else
            return InlineVector<T, CompileTimeCapacityOrSize>(runtime_size, 0);
    } else
        return std::vector<T>(runtime_size, 0);
}

template <class T, size_t CompileTimeCapacityOrSize, bool HasCompileTimeSize>
constexpr auto make_uninitialized_array_or_inlinevector_or_vector(
    size_t runtime_size)
{
    if constexpr (CompileTimeCapacityOrSize != c_runtime_size_marker) {
        if constexpr (HasCompileTimeSize)
            return std::array<T, CompileTimeCapacityOrSize>();
        else
            return InlineVector<T, CompileTimeCapacityOrSize>(runtime_size,
                                                              uninitialized);
    } else
        return std::vector<T>(runtime_size,
                              0);  // no unitialized vector available
}

// ----------------------------------

template <class Subtype>
struct size_bounds_expression
{
    constexpr static int compile_time_capacity = Subtype::compile_time_capacity;
    constexpr static int compile_time_size = Subtype::compile_time_size;

    int runtime_size() const { return as_subtype().runtime_size(); }

    const Subtype& as_subtype() const
    {
        return *static_cast<const Subtype*>(this);
    }
};

template <int Size>
struct array_like_size_bounds
    : size_bounds_expression<array_like_size_bounds<Size>>
{
    constexpr static int compile_time_capacity = Size;
    constexpr static int compile_time_size = Size;

    int runtime_size() const { return Size; }
};

template <int Capacity>
struct inlinevector_like_size_bounds
    : size_bounds_expression<inlinevector_like_size_bounds<Capacity>>
{
    constexpr static int compile_time_capacity = Capacity;
    constexpr static int compile_time_size = c_runtime_size_marker;

    explicit inlinevector_like_size_bounds(int size) : size(size) {}

    int runtime_size() const { return size; }

private:
    const int size;
};

struct vector_like_size_bounds : size_bounds_expression<vector_like_size_bounds>
{
    constexpr static int compile_time_capacity = c_runtime_size_marker;
    constexpr static int compile_time_size = c_runtime_size_marker;

    explicit vector_like_size_bounds(int size) : size(size) {}

    int runtime_size() const { return size; }

private:
    const int size;
};

template <class X, class Y>
struct size_bounds_sum : size_bounds_expression<size_bounds_sum<X, Y>>
{
    constexpr static int compile_time_capacity =
        (X::compile_time_capacity == c_runtime_size_marker ||
         Y::compile_time_capacity == c_runtime_size_marker)
            ? c_runtime_size_marker
            : X::compile_time_capacity + Y::compile_time_capacity;
    constexpr static int compile_time_size =
        (X::compile_time_size == c_runtime_size_marker ||
         Y::compile_time_size == c_runtime_size_marker)
            ? c_runtime_size_marker
            : X::compile_time_size + Y::compile_time_size;

    size_bounds_sum(const X& x, const Y& y)
        : size(x.runtime_size() + y.runtime_size())
    {}

    int runtime_size() const { return size; }

private:
    const int size;
};

template <class X, class Y>
struct size_bounds_product : size_bounds_expression<size_bounds_product<X, Y>>
{
    constexpr static int compile_time_capacity =
        (X::compile_time_capacity == c_runtime_size_marker ||
         Y::compile_time_capacity == c_runtime_size_marker)
            ? c_runtime_size_marker
            : X::compile_time_capacity * Y::compile_time_capacity;
    constexpr static int compile_time_size =
        (X::compile_time_size == c_runtime_size_marker ||
         Y::compile_time_size == c_runtime_size_marker)
            ? c_runtime_size_marker
            : X::compile_time_size * Y::compile_time_size;

    size_bounds_product(const X& x, const Y& y)
        : size(x.runtime_size() * y.runtime_size())
    {}

    int runtime_size() { return size; }

private:
    const int size;
};

template <class X, class Y>
struct size_bounds_min : size_bounds_expression<size_bounds_min<X, Y>>
{
    // For capacity we prefer a conservative estimation, so the
    // compile_time_capacity is not a strict min, if one is compile time
    // and the other is runtime, the compile-time value will be returned in
    // compile-time.
    constexpr static int compile_time_capacity =
        X::compile_time_capacity == c_runtime_size_marker
            ? (Y::compile_time_capacity == c_runtime_size_marker
                   ? c_runtime_size_marker
                   : Y::compile_time_capacity)
            : (Y::compile_time_capacity == c_runtime_size_marker
                   ? X::compile_time_capacity
                   : std::min(X::compile_time_capacity,
                              Y::compile_time_capacity));
    // For size we need exact values
    constexpr static int compile_time_size =
        (X::compile_time_size == c_runtime_size_marker ||
         Y::compile_time_size == c_runtime_size_marker)
            ? c_runtime_size_marker
            : std::min(X::compile_time_size, Y::compile_time_size);

    size_bounds_min(const X& x, const Y& y)
        : size(std::min(x.runtime_size(), y.runtime_size()))
    {}

    int runtime_size() const { return size; }

private:
    const int size;
};

template <class X, class Y>
struct size_bounds_max : size_bounds_expression<size_bounds_max<X, Y>>
{
    constexpr static int compile_time_capacity =
        (X::compile_time_capacity == c_runtime_size_marker ||
         Y::compile_time_capacity == c_runtime_size_marker)
            ? c_runtime_size_marker
            : std::max(X::compile_time_capacity, Y::compile_time_capacity);
    constexpr static int compile_time_size =
        (X::compile_time_size == c_runtime_size_marker ||
         Y::compile_time_size == c_runtime_size_marker)
            ? c_runtime_size_marker
            : std::max(X::compile_time_size, Y::compile_time_size);

    size_bounds_max(const X& x, const Y& y)
        : size(std::max(x.runtime_size(), y.runtime_size()))
    {}

    int runtime_size() const { return size; }

private:
    const int size;
};

template <class X>
struct size_bounds_negation : size_bounds_expression<size_bounds_negation<X>>
{
    constexpr static int compile_time_capacity =
        X::compile_time_capacity == c_runtime_size_marker
            ? c_runtime_size_marker
            : -X::compile_time_capacity;
    constexpr static int compile_time_size =
        X::compile_time_size == c_runtime_size_marker ? c_runtime_size_marker
                                                      : -X::compile_time_size;

    explicit size_bounds_negation(const X& x) : size(-x.runtime_size()) {}

    int runtime_size() const { return size; }

private:
    const int size;
};

template <int X>
struct size_bounds_constant : size_bounds_expression<size_bounds_constant<X>>
{
    constexpr static int compile_time_capacity = X;
    constexpr static int compile_time_size = X;

    constexpr int runtime_size() const { return X; }
};

template <class X, class Y>
auto operator+(const size_bounds_expression<X>& x,
               const size_bounds_expression<Y>& y)
{
    return size_bounds_sum<X, Y>(x.as_subtype(), y.as_subtype());
}

template <class X, class Y>
auto operator-(const size_bounds_expression<X>& x,
               const size_bounds_expression<Y>& y)
{
    return size_bounds_sum<X, size_bounds_negation<Y>>(
        x.as_subtype(), size_bounds_negation<Y>(y.as_subtype()));
}

template <class X, class Y>
auto operator*(const size_bounds_expression<X>& x,
               const size_bounds_expression<Y>& y)
{
    return size_bounds_product<X, Y>(x.as_subtype(), y.as_subtype());
}

template <class X, class Y>
auto min(const size_bounds_expression<X>& x, const size_bounds_expression<Y>& y)
{
    return size_bounds_min<X, Y>(x.as_subtype(), y.as_subtype());
}

template <class X, class Y>
auto max(const size_bounds_expression<X>& x, const size_bounds_expression<Y>& y)
{
    return size_bounds_max<X, Y>(x.as_subtype(), y.as_subtype());
}

template <class X>
struct is_array : std::false_type
{
};

template <class T, size_t N>
struct is_array<std::array<T, N>> : std::true_type
{
};

template <class X>
struct is_inlinevector : std::false_type
{
};

template <class T, size_t N>
struct is_inlinevector<InlineVector<T, N>> : std::true_type
{
};

template <class T, size_t N>
auto get_array_size_bounds(const std::array<T, N>&)
{
    return array_like_size_bounds<N>();
}

template <class T, int N>
auto get_inlinevector_size_bounds(const InlineVector<T, N>& x)
{
    return inlinevector_like_size_bounds<N>(x.size());
}

template <class X>
auto get_size_bounds(const X& x)
{
    if constexpr (is_array<X>::value)
        return get_array_size_bounds(x);
    else if constexpr (is_inlinevector<X>::value)
        return get_inlinevector_size_bounds(x);
    else
        return vector_like_size_bounds(x.size());
}

template <class T, class X>
constexpr auto make_zero_initialized_array_or_inlinevector_or_vector(
    const size_bounds_expression<X>& x)
{
    if constexpr (X::compile_time_size != c_runtime_size_marker)
        return std::array<T, X::compile_time_size>{};
    else if constexpr (X::compile_time_capacity != c_runtime_size_marker)
        return InlineVector<T, X::compile_time_capacity>(x.runtime_size(), 0);
    else
        return std::vector<T>(x.runtime_size(), 0);
}

template <class T, class X>
constexpr auto make_uninitialized_array_or_inlinevector_or_vector(
    const size_bounds_expression<X>& x)
{
    if constexpr (X::compile_time_size != c_runtime_size_marker)
        return std::array<T, X::compile_time_size>();
    else if constexpr (X::compile_time_capacity != c_runtime_size_marker)
        return InlineVector<T, X::compile_time_capacity>(x.runtime_size(),
                                                         uninitialized);
    else
        return std::vector<T>(
            x.runtime_size());  // no uninitialized vector available
}

}  // namespace ul
