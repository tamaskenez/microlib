#pragma once

namespace ul {

#define DEF_BINOP(NAME, FUN)                                                 \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<X>::value == range_code<Y>::value)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<decltype(FUN(x[0], y[0])), range_code<X>::value>             \
    {                                                                        \
        array<decltype(FUN(x[0], y[0])), range_code<X>::value> r;            \
        for (int i = 0; i < range_code<X>::value; ++i)                       \
            r[i] = FUN(x[i], y[i]);                                          \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value >= 0 &&                    \
                             range_code<Y>::value == c_range_code_unknown)>  \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<decltype(FUN(x[0], y)), range_code<X>::value>                \
    {                                                                        \
        array<decltype(FUN(x[0], y)), range_code<X>::value> r;               \
        for (int i = 0; i < range_code<X>::value; ++i)                       \
            r[i] = FUN(x[i], y);                                             \
        return r;                                                            \
    }                                                                        \
    template <class X, class Y,                                              \
              UL_T_ENABLE_IF(range_code<X>::value == c_range_code_unknown && \
                             range_code<Y>::value >= 0)>                     \
    auto NAME(const X& x, const Y& y)                                        \
        ->array<decltype(FUN(x, y[0])), range_code<Y>::value>                \
    {                                                                        \
        array<decltype(FUN(x, y[0])), range_code<Y>::value> r;               \
        for (int i = 0; i < range_code<Y>::value; ++i)                       \
            r[i] = FUN(x, y[i]);                                             \
        return r;                                                            \
    }

DEF_BINOP(operator+, operator+)
DEF_BINOP(operator-, operator-)
DEF_BINOP(operator*, operator*)
DEF_BINOP(min, std::min)
DEF_BINOP(max, std::max)

#undef DEF_BINOP
}
