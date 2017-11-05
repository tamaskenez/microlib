#pragma once

#include <functional>

#include "ul/inlinevector.h"

namespace ul {

// type_traits-style template-meta-horror for creating polymorph functions
// which could take and return views and containers of various compile-time
// / runtime size characteristics.
//
// That is, a function, depending on whether the input parameters (ranges,
// views, containers, whatever) have compile-time or runtime capacity or
// size could return an array, InlineVector or vector.

// There are two solutions here, first one is deprecated, it's more complicated
// to use but maybe easier to implement with C++11-like features.

// The second one is a relatively simple implementation and much simpler to use.
// See below.

// #### COMMON CODE FOR BOTH SOLUTIONS ####

// Note: we use `int` for everything to avoid surprising wrapovers in
// calculations (sometimes we need things like (max(size1 + size2 - 1, 0))

// unique value meaning i-don't-know-this-in-compile-time
static constexpr int c_runtime_size_marker = INT_MAX;

// #### DEPRECATED SOLUTION ####

// First we need to know if a particular sequence type has runtime or
// compile-time capacity and size. This trait type is to be extended
// with new types as they are introduced, for example, we could have
// a fixed-compile-time sized span in the future.
// For now:
//
// - std::array has compile-time capacity and size
// - ul::InlineVector has compile-time capacity but runtime size
// - std::vector: all runtime

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

// These functions return a container that satisfies the requirements and may be
// initialized to zero.
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

// #### IMPROVED SOLUTION ####

// This solution introduces the concept of a weird type that contains two
// integers, a capacity and a size value. The type is weird because it can
// describe the capacity and size of all these types:
//
// - std::array, with compile-type size (which is its capacity, too)
// - ul::InlineVector, with compile-type capacity and runtime size
// - std::vector, with runtime capacity and runtime size
//
// You can create expressions with this type like `min(x + y - 1, 0)` and ask
// the result of the expression both in compile time and in runtime. The result
// will depend on the source of the x and y variables, that is, whether they
// represent the capacity/size of an array, InlineVector or vector (or span).
//
// For the capacity value, in compile time you either the actual capacity
// (std::array and ul::InlineVector) or the special value c_runtime_size_marker.
//
// For the size value, in compile time you either get the actual size
// (std::array) or the special value c_runtime_size_marker. Then, in runtime you
// can ask the actual runtime size which is always a concrete value.
//
// Example:
//
// template<X, Y>
// auto myfun(const X& x, const Y& y) {
//     auto x_bounds = get_size_bounds(x);
//     auto y_bounds = get_size_bounds(x);
//     auto result_bounds = min(x + y - size_bounds_constant(1));
//     auto result =
//       make_zero_initialized_array_or_inlinevector_or_vector(result_bounds);
//     for(...) result[i] = ...;
//     return result;
// }

template <int Capacity, int Size>
struct size_bounds
{
    constexpr static int compile_time_capacity = Capacity;
    constexpr static int compile_time_size = Size;

    explicit size_bounds(int s) : s(s) {}
    int runtime_size() const { return s; }

private:
    const int s;
};

// A constant that describes a compile-time known capacity == size values.
template <int X>
struct size_bounds_constant : size_bounds<X, X>
{
    size_bounds_constant() : size_bounds<X, X>(X) {}
};

// A constant that describes compile-time capacity and runtime size.
template <int Capacity>
struct inlinevector_like_size_bounds
    : size_bounds<Capacity, c_runtime_size_marker>
{
    explicit inlinevector_like_size_bounds(int size)
        : size_bounds<Capacity, c_runtime_size_marker>(size)
    {}
};

// A constant that describes runtime capacity (ignored) and size.
struct vector_like_size_bounds
    : size_bounds<c_runtime_size_marker, c_runtime_size_marker>
{
    explicit vector_like_size_bounds(int size) : size_bounds(size) {}
};

template <class F>
static constexpr int eval_size_bounds_op(int x, int y)
{
    return x == c_runtime_size_marker || y == c_runtime_size_marker
               ? c_runtime_size_marker
               : F()(x, y);
}

template <class F, class X, class Y>
auto make_size_bounds_binary_op(const X& x, const Y& y)
{
    return size_bounds<eval_size_bounds_op<F>(X::compile_time_capacity,
                                              Y::compile_time_capacity),
                       eval_size_bounds_op<F>(X::compile_time_size,
                                              Y::compile_time_size)>(
        F()(x.runtime_size(), y.runtime_size()));
}

template <int A, int B, int C, int D>
auto operator+(const size_bounds<A, B>& x, const size_bounds<C, D>& y)
{
    return make_size_bounds_binary_op<std::plus<int>>(x, y);
}

template <int A, int B, int C, int D>
auto operator-(const size_bounds<A, B>& x, const size_bounds<C, D>& y)
{
    return make_size_bounds_binary_op<std::minus<int>>(x, y);
}

template <int A, int B, int C, int D>
auto operator*(const size_bounds<A, B>& x, const size_bounds<C, D>& y)
{
    return make_size_bounds_binary_op<std::multiplies<int>>(x, y);
}

struct size_bounds_function_object_min
{
    constexpr int operator()(int x, int y) const { return std::min(x, y); }
};
struct size_bounds_function_object_max
{
    constexpr int operator()(int x, int y) const { return std::max(x, y); }
};

template <int A, int B, int C, int D>
auto min(const size_bounds<A, B>& x, const size_bounds<C, D>& y)
{
    return make_size_bounds_binary_op<size_bounds_function_object_min>(x, y);
}

template <int A, int B, int C, int D>
auto max(const size_bounds<A, B>& x, const size_bounds<C, D>& y)
{
    return make_size_bounds_binary_op<size_bounds_function_object_max>(x, y);
}

// Trait types for testing the nature of the container
template <class X>
struct is_inlinevector : std::false_type
{
};

template <class T, size_t N>
struct is_inlinevector<InlineVector<T, N>> : std::true_type
{
};

// Return the appropriate expression describing the capacity/size
// characteristics for a container.
template <class T, size_t N>
auto get_array_size_bounds(const std::array<T, N>&)
{
    return size_bounds_constant<N>();
}

template <class T, int N>
auto get_inlinevector_size_bounds(const InlineVector<T, N>& x)
{
    return inlinevector_like_size_bounds<N>(x.size());
}

template <class X>
auto get_size_bounds(const X& x)
{
    if constexpr (is_std_array<X>::value)
        return get_array_size_bounds(x);
    else if constexpr (is_inlinevector<X>::value)
        return get_inlinevector_size_bounds(x);
    else
        return vector_like_size_bounds(x.size());
}

// Create zero-initialized or uninitialized containers from a size_bounds
// expression with an appropriate container.
template <class T, class X>
constexpr auto make_zero_initialized_array_or_inlinevector_or_vector(const X& x)
{
    if constexpr (X::compile_time_size != c_runtime_size_marker)
        return std::array<T, X::compile_time_size>{};
    else if constexpr (X::compile_time_capacity != c_runtime_size_marker)
        return InlineVector<T, X::compile_time_capacity>(x.runtime_size(), 0);
    else
        return std::vector<T>(x.runtime_size(), 0);
}

template <class T, class X>
constexpr auto make_uninitialized_array_or_inlinevector_or_vector(const X& x)
{
    if constexpr (X::compile_time_size != c_runtime_size_marker)
        return std::array<T, X::compile_time_size>();
    else if constexpr (X::compile_time_capacity != c_runtime_size_marker)
        return InlineVector<T, X::compile_time_capacity>(x.runtime_size(),
                                                         uninitialized);
    else
        return std::vector<T>(x.runtime_size());  // it's difficult to create
                                                  // uninitialized vector
                                                  // (custom allocator
                                                  // business) so here we
                                                  // supply the regular,
                                                  // zero-initialized vector
}

}  // namespace ul
