#undef NDEBUG

#include <cassert>
#include <cstdio>
#include <functional>
#include <string>

#include "ul/alg_elementwise.h"

#include "test_common.cpp"

#define GO(EO, O)                    \
    z = x;                           \
    ULCC EWM(z) EO y;                \
    for (int i = 0; i < N; ++i)      \
        assert(z[i] == x[i] O y[i]); \
    z = x;                           \
    ULCC EWM(z) EO y[0];             \
    for (int i = 0; i < N; ++i)      \
        assert(z[i] == x[i] O y[0]);

#define GE(O)                           \
    bs = ULCC EW(x) O ULCC EW(y);       \
    for (int i = 0; i < N; ++i)         \
        assert(bs[i] == (x[i] O y[i])); \
    bs = ULCC EW(x) O y[0];             \
    for (int i = 0; i < N; ++i)         \
        assert(bs[i] == (x[i] O y[0])); \
    bs = x[0] O ULCC EW(y);             \
    for (int i = 0; i < N; ++i)         \
        assert(bs[i] == (x[0] O y[i]));

template <int N>
void f(int a, int b, int c)
{
    std::array<int, N> x, y, z;
    std::bitset<N> bs;
    int counter = 0;
    for (auto& i : x)
        i = 5 + (counter++ % 10);
    for (auto& i : y)
        i = 5 + (counter++ % 10);
    x.front() = a;
    x[x.size() / 2] = b;
    x.back() = c;
    GO(+=, +)
    GO(-=, -)
    GO(*=, *)
    GO(/=, /)
    GE(==)
    GE(!=)
    GE(<=)
    GE(>=)
    GE(>)
    GE(<)
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
