#pragma once

#include <algorithm>
#include <functional>

#include "ul/type_traits.h"

/*
WARNING: this header overloads some operators (+ - * /) in the ul
namespace. The types it overloads for are controlled by
ul::range_code<X>. The 'using namespace ul' puts these general
overloads into the current namespace, so be careful.

Functions and operators with the signature:

    container = x OP y
    container = function(x, y)

where x and y are ranges (types, identified by ul::range_code<X>)
and container is either std::array or std::vector, which depends
on the nature of x and y (have compile-time or runtime sizes)

The following operations are defined:

    - operators + - * / (element-wise)
    - min_ew, max_ew (element-wise min and max)

For now only the compile-time size stuff is implemented.

*/

namespace ul {

using std::array;

#define DEF_BINOP(NAME, OP)                                                  \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<X>::value == range_code<Y>::value)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(x[0] OP y[0]), range_code<X>::value>            \
    {                                                                        \
        array<UL_DECAYDECL(x[0] OP y[0]), range_code<X>::value> r;           \
        for (int i = 0; i < range_code<X>::value; ++i)                       \
            r[i] = x[i] OP y[i];                                             \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<Y>::value == c_range_code_unknown)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(x[0] OP y), range_code<X>::value>               \
    {                                                                        \
        array<UL_DECAYDECL(x[0] OP y), range_code<X>::value> r;              \
        for (int i = 0; i < range_code<X>::value; ++i)                       \
            r[i] = x[i] OP y;                                                \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value == c_range_code_unknown && \
                             range_code<Y>::value >= 0)>                     \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(x OP y[0]), range_code<Y>::value>               \
    {                                                                        \
        array<UL_DECAYDECL(x OP y[0]), range_code<Y>::value> r;              \
        for (int i = 0; i < range_code<Y>::value; ++i)                       \
            r[i] = x OP y[i];                                                \
        return r;                                                            \
    }

#define DEF_BINFUN(NAME, FUN)                                                \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<X>::value == range_code<Y>::value)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(FUN(x[0], y[0])), range_code<X>::value>         \
    {                                                                        \
        array<UL_DECAYDECL(FUN(x[0], y[0])), range_code<X>::value> r;        \
        for (int i = 0; i < range_code<X>::value; ++i) {                     \
            r[i] = FUN(x[i], y[i]);                                          \
        }                                                                    \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<Y>::value == c_range_code_unknown)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(FUN(x[0], y)), range_code<X>::value>            \
    {                                                                        \
        array<UL_DECAYDECL(FUN(x[0], y)), range_code<X>::value> r;           \
        for (int i = 0; i < range_code<X>::value; ++i)                       \
            r[i] = FUN(x[i], y);                                             \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value == c_range_code_unknown && \
                             range_code<Y>::value >= 0)>                     \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<UL_DECAYDECL(FUN(x, y[0])), range_code<Y>::value>            \
    {                                                                        \
        array<UL_DECAYDECL(FUN(x, y[0])), range_code<Y>::value> r;           \
        for (int i = 0; i < range_code<Y>::value; ++i)                       \
            r[i] = FUN(x, y[i]);                                             \
        return r;                                                            \
    }

DEF_BINOP(operator+, +)
DEF_BINOP(operator-, -)
DEF_BINOP(operator*, *)
DEF_BINOP(operator/, /)
DEF_BINFUN(min_ew, std::min)
DEF_BINFUN(max_ew, std::max)

#undef DEF_BINOP
}
