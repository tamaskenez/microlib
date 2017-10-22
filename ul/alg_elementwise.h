#pragma once

/*
Helper constructs

- to allow +=, -=, *=, /= operators on existing types
- to allow element-wise ==, !=, <=, >=, <, > on existing types

For now, only std::array is supported:

    array<int, 3> x, y;

    EWM(x) *= 3;
    EWM(x) += y;

    bool b = any_of(EW(x) >= EW(y));
*/

#include <bitset>

#include "ul/type_traits.h"

namespace ul {

using std::array;
using std::bitset;

template <class T, size_t N>
class EWArrayRef  // element-wise array ref
{
public:
    explicit EWArrayRef(const array<T, N>& x) : x(&x) {}
    const array<T, N>& get() const { return *x; }

private:
    const array<T, N>* x;
};

template <class T, size_t N>
class EWArrayRefM  // element-wise mutable array ref
{
public:
    explicit EWArrayRefM(array<T, N>& x) : x(&x) {}
    array<T, N>& get() const { return *x; }
#define DEFOP(OPEQ)                          \
    void operator OPEQ(const array<T, N>& y) \
    {                                        \
        for (int i = 0; i < N; ++i)          \
            (*x)[i] OPEQ y[i];               \
    }
    DEFOP(+=)
    DEFOP(-=)
    DEFOP(*=)
    DEFOP(/=)
#undef DEFOP
#define DEFOP(OPEQ)                 \
    void operator OPEQ(T y)         \
    {                               \
        for (int i = 0; i < N; ++i) \
            (*x)[i] OPEQ y;         \
    }
    DEFOP(+=)
    DEFOP(-=)
    DEFOP(*=)
    DEFOP(/=)
#undef DEFOP
private:
    array<T, N>* x;
};

template <class T, size_t N>
EWArrayRef<T, N> EW(const array<T, N>& x)
{
    return EWArrayRef<T, N>(x);
}

template <class T, size_t N>
EWArrayRefM<T, N> EWM(array<T, N>& x)
{
    return EWArrayRefM<T, N>(x);
}

#define DEFOP(OP)                                                 \
    template <class T, size_t N>                                  \
    bitset<N> operator OP(EWArrayRef<T, N> x, EWArrayRef<T, N> y) \
    {                                                             \
        bitset<N> r;                                              \
        for (int i = 0; i < N; ++i)                               \
            r.set(i, x.get()[i] OP y.get()[i]);                   \
        return r;                                                 \
    }                                                             \
    template <class T, size_t N>                                  \
    bitset<N> operator OP(EWArrayRef<T, N> x, T y)                \
    {                                                             \
        bitset<N> r;                                              \
        for (int i = 0; i < N; ++i)                               \
            r.set(i, x.get()[i] OP y);                            \
        return r;                                                 \
    }                                                             \
    template <class T, size_t N>                                  \
    bitset<N> operator OP(T x, EWArrayRef<T, N> y)                \
    {                                                             \
        bitset<N> r;                                              \
        for (int i = 0; i < N; ++i)                               \
            r.set(i, x OP y.get()[i]);                            \
        return r;                                                 \
    }
DEFOP(<)
DEFOP(>)
DEFOP(==)
DEFOP(!=)
DEFOP(<=)
DEFOP(>=)
#undef DEFOP
}  // namespace ul
