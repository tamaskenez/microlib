#undef NDEBUG

#include <algorithm>
#include <cassert>
#include <climits>
#include <numeric>

#include "ul/ml.h"
#include "ul/span.h"
#include "ul/ul.h"

using std::array;
using std::vector;

using ul::conv;
using ul::conv_into;
using ul::db2mag;
using ul::db2pow;
using ul::mag2db;
using ul::make_span;
using ul::polyder;
using ul::polyint;
using ul::polyval;
using ul::pow2db;

template <int N>
using IV = ul::InlineVector<int, N>;

template <class ExpectedResultType, class T>
void test_polyder(const T& x)
{
    std::array<int, 3> expected{{345, 468, 369}};
    auto r = polyder(x);
    static_assert(std::is_same<decltype(r), ExpectedResultType>::value);
    assert(r.size() + 1 == x.size());
    assert(std::equal(BE(r), BE(expected)));
}

template <class ExpectedResultType, class T>
void test_polyint(const T& x, int C0)
{
    std::array<int, 4> expected{{456, 345, 117, 41}};
    auto r = polyint(x, C0);
    static_assert(std::is_same<decltype(r), ExpectedResultType>::value);
    assert(r.size() == x.size() + 1);
    assert(std::equal(BE(r), BE(expected)));
}

template <class ExpectedResultType, class X, class Y>
void test_conv(const X& x, const Y& y)
{
    std::array<int, 7> expected = {
        {1476, 5637, 13704, 21363, 29022, 28545, 19278}};
    auto r = conv(x, y);
    static_assert(std::is_same<decltype(r), ExpectedResultType>::value);
    assert(r.size() == x.size() + y.size() - 1);
    assert(std::equal(BE(r), BE(expected)));
    std::fill(BE(r), 999999);
    if constexpr (!ul::is_std_array<ExpectedResultType>::value)
        r.resize(r.size() / 2);
    conv_into(x, y, r);
    assert(std::equal(BE(r), BE(expected)));
}

template <class X, class Y, class Z>
void test_conv()
{
    X x{{123, 234, 345, 456, 567}};
    Y y{{12, 23, 34}};
    test_conv<Z>(x, y);
}

int main()
{
    array<int, 5> c{{2, -3, -4, 5, 6}};
    for (int x = -3; x <= 3; ++x) {
        auto ya = polyval(make_span(c.data(), 1), x);
        auto yb = c[0];
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 2), x);
        yb += c[1] * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 3), x);
        yb += c[2] * x * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 4), x);
        yb += c[3] * x * x * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 5), x);
        yb += c[4] * x * x * x * x;
        assert(ya == yb);
    }

    {
#define POLYDER_X {456, 345, 234, 123}
        std::array<int, 4> a{POLYDER_X};
        ul::InlineVector<int, 7> iv(POLYDER_X);
        std::vector<int> v{POLYDER_X};
        test_polyder<std::array<int, 3>>(a);
        test_polyder<ul::InlineVector<int, 6>>(iv);
        test_polyder<std::vector<int>>(v);
        test_polyder<std::vector<int>>(ul::as_span(a));
    }

    {
#define POLYINT_X {345, 234, 123}
        std::array<int, 3> a{POLYINT_X};
        ul::InlineVector<int, 7> iv(POLYINT_X);
        std::vector<int> v{POLYINT_X};
        test_polyint<std::array<int, 4>>(a, 456);
        test_polyint<ul::InlineVector<int, 8>>(iv, 456);
        test_polyint<std::vector<int>>(v, 456);
        test_polyint<std::vector<int>>(ul::as_span(a), 456);
    }

    {
        using A7 = std::array<int, 7>;
        using A5 = std::array<int, 5>;
        using A3 = std::array<int, 3>;
        using V = std::vector<int>;

        test_conv<IV<7>, IV<9>, IV<15>>();
        test_conv<IV<7>, A3, IV<9>>();
        test_conv<IV<7>, V, V>();
        test_conv<A5, IV<9>, IV<13>>();
        test_conv<A5, A3, A7>();
        test_conv<A5, V, V>();
        test_conv<V, IV<9>, V>();
        test_conv<V, A3, V>();
        test_conv<V, V, V>();
    }

    {
        assert(fabs(pow2db(1) - 0) < 1e-12);
        assert(fabs(pow2db(100) - 20) < 1e-12);
        assert(fabs(db2pow(0) - 1) < 1e-12);
        assert(fabs(db2pow(20) - 100) < 1e-12);
        assert(fabs(mag2db(1) - 0) < 1e-12);
        assert(fabs(mag2db(10) - 20) < 1e-12);
        assert(fabs(db2mag(0) - 1) < 1e-12);
        assert(fabs(db2mag(20) - 10) < 1e-12);
    }

    printf("Done.\n");
    return 0;
}
