#undef NDEBUG

#include <cassert>
#include <cstdio>
#include <functional>

#include "ul/alg_cont_eq_fun.h"

#include "test_common.cpp"

#define GF(ULFUN, FUN)                   \
    z = ULFUN(x, y);                     \
    for (int i = 0; i < N; ++i)          \
        assert(z[i] == FUN(x[i], y[i])); \
    z = ULFUN(x, y[0]);                  \
    for (int i = 0; i < N; ++i)          \
        assert(z[i] == FUN(x[i], y[0])); \
    z = ULFUN(x[0], y);                  \
    for (int i = 0; i < N; ++i)          \
        assert(z[i] == FUN(x[0], y[i]));

#define GO(O)                        \
    z = x O y;                       \
    for (int i = 0; i < N; ++i)      \
        assert(z[i] == x[i] O y[i]); \
    z = x O y[0];                    \
    for (int i = 0; i < N; ++i)      \
        assert(z[i] == x[i] O y[0]); \
    z = x[0] O y;                    \
    for (int i = 0; i < N; ++i)      \
        assert(z[i] == x[0] O y[i]);

template <int N>
void f(int a, int b, int c)
{
    std::array<int, N> x, y, z;
    int counter = 0;
    for (auto& i : x)
        i = 5 + (counter++ % 10);
    for (auto& i : y)
        i = 5 + (counter++ % 10);
    x.front() = a;
    x[x.size() / 2] = b;
    x.back() = c;
#ifndef ULTEST_USING_UL
    using ul::operator+;
    using ul::operator*;
    using ul::operator-;
    using ul::operator/;
#endif
    GO(+)
    GO(-)
    GO(*)
    GO(/)
    GF(ULCC min_ew, STDCC min)
    GF(ULCC max_ew, STDCC max)
}

int main()
{
    for (int a = 1; a <= 21; a += 10) {
        for (int b = 1; b <= 21; b += 10)
            for (int c = 1; c <= 21; c += 10) {
                f<1>(a, b, c);
                f<2>(a, b, c);
                f<4>(a, b, c);
                f<23>(a, b, c);
            }
    }
    printf("Done.\n");
}
