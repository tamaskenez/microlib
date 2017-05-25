#pragma once

namespace ul {

#define DEF_FOLD(NAME, FUN)                                                   \
    template <class T,                                                        \
              UL_T_ENABLE_IF(range_code<T>::value >= c_range_code_indexable)> \
    auto NAME(const T& v)->decltype(v[0])                                     \
    {                                                                         \
        assert(v.size() > 0);                                                 \
        decltype(v[0]) r(v[0]);                                               \
        for (ptrdiff_t i = 1; i < (ptrdiff_t)v.size(); ++i)                   \
            r = FUN(r, v[i]);                                                 \
        return r;                                                             \
    }

DEF_FOLD(sum, operator+)
DEF_FOLD(prod, operator*)
DEF_FOLD(min, std::min)
DEF_FOLD(max, std::max)
#undef DEF_FOLD
}
