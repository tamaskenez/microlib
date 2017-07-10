#undef NDEBUG

#include <cassert>

#include "ul/alg_scalar_eq_fun.h"

#include "test_common.cpp"

#if 0
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

#endif

template <int N>
void f(int a, int b, int c)
{
    std::array<int, N> x;
    std::vector<int> y(N);

    int counter = 0;
    for (auto& i : x)
        i = 5 + (counter++ % 10);

    counter = 0;
    for (auto& i : y)
        i = 5 + (counter++ % 10);

    x.front() = a;
    x[x.size() / 2] = b;
    x.back() = c;
    y.front() = a;
    y[y.size() / 2] = b;
    y.back() = c;

    int xsum = 0, xprod = 1, xmin = INT_MAX, xmax = INT_MIN;
    for (auto& i : x) {
        xsum += i;
        xprod *= i;
        xmin = std::min(xmin, i);
        xmax = std::max(xmax, i);
    }

    assert(xsum == ULCC sum(x));
    assert(xprod == ULCC prod(x));
    assert(xmin == ULCC min(x));
    assert(xmax == ULCC max(x));
    assert(xsum == ULCC sum(y));
    assert(xprod == ULCC prod(y));
    assert(xmin == ULCC min(y));
    assert(xmax == ULCC max(y));
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
