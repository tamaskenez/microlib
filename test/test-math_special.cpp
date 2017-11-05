#undef NDEBUG

#include <algorithm>
#include <cassert>
#include <climits>
#include <numeric>

#include "ul/math_special.h"
#include "ul/ml.h"
#include "ul/type_traits.h"
#include "ul/ul.h"

using std::vector;

using ul::as_span;
using ul::diffcos;
using ul::diffsin;
using ul::difftan;
using ul::make_span;
using ul::polycompose;
using ul::polyval;
using ul::sign;
using ul::span;
using ul::within_co;

void assert_approx_eq(double x, double y, double eps)
{
    bool ok = fabs(x - y) < eps;
    if (!ok) {
        fprintf(stderr, "!approx_eq: %f and %f, diff: %g\n", x, y, fabs(x - y));
        assert(false);
    }
}

template <class F>
double approx_deriv(F&& f, double x)
{
    const double d = 1e-5;
    const double num = (f(x + d) - f(x - d));
    return num / (2 * d);
}

template <class F>
double approx_deriv(int deg, F&& ff, double x)
{
    // http://web.media.mit.edu/~crtaylor/calculator.html
    const int npoints = 5;
    std::array<double, npoints> f;
    const int i = (npoints - 1) / 2;
    const double h = 1e-4;
    for (int i = 0; i < 5; ++i)
        f[i] = ff(x + (i - 2) * h);
    if (deg == 1) {
        return (1 * f[i - 2] - 8 * f[i - 1] + 0 * f[i + 0] + 8 * f[i + 1] -
                1 * f[i + 2]) /
               (12 * 1.0 * h);
    }
    if (deg == 2) {
        return (-1 * f[i - 2] + 16 * f[i - 1] - 30 * f[i + 0] + 16 * f[i + 1] -
                1 * f[i + 2]) /
               (12 * 1.0 * h * h);
    }
    if (deg == 3) {
        return (-1 * f[i - 2] + 2 * f[i - 1] + 0 * f[i + 0] - 2 * f[i + 1] +
                1 * f[i + 2]) /
               (2 * 1.0 * pow(h, 3));
    }
    assert(false);
}

template <class F>
std::function<double(double)> approx_deriv(F&& f)
{
    return [f](double x) { return approx_deriv(f, x); };
}

constexpr int pcn(int x, int y)
{
    return (x - 1) * (y - 1) + 1;
}

template <class ExpectedResultType, class P, class Q>
void test_polycompose(P& p, Q& q)
{
    FOR(i, 0, < p.size()) p[i] = p.size() - i + 1;
    FOR(i, 0, < q.size()) q[i] = q.size() - i + 1;
    assert(p.back() == 2);
    assert(q.back() == 2);
    FOR(phase, 1, <= 2)
    {
        auto pq = polycompose(p, q);
        static_assert(std::is_same<ExpectedResultType, decltype(pq)>::value);
        assert(pq.size() == pcn(p.size(), q.size()));

        FOR(x, -4, <= 4)
        {
            auto pqx_expected = polyval(p, polyval(q, x));
            auto pqx_actual = polyval(pq, x);
            assert(pqx_actual == pqx_expected);
        }
        for (auto& i : p)
            i = -i;
        for (auto& i : q)
            i = -i;
    }
}

template <size_t N>
using A = std::array<int, N>;
template <int N>
using IV = ul::InlineVector<int, N>;
using V = std::vector<int>;

template <int NP, int NQ>
void test_polycompose()
{
    A<NP> pa;
    A<NQ> qa;
    const int NPC = NP + 2;
    const int NQC = NQ + 3;
    IV<NPC> piv(NP, ul::uninitialized);
    IV<NQC> qiv(NQ, ul::uninitialized);
    V pv(NP), qv(NQ);

    test_polycompose<A<pcn(NP, NQ)>>(pa, qa);
    test_polycompose<IV<pcn(NP, NQC)>>(pa, qiv);
    test_polycompose<V>(pa, qv);
    test_polycompose<IV<pcn(NPC, NQ)>>(piv, qa);
    test_polycompose<IV<pcn(NPC, NQC)>>(piv, qiv);
    test_polycompose<V>(piv, qv);
    test_polycompose<V>(pv, qa);
    test_polycompose<V>(pv, qiv);
    test_polycompose<V>(pv, qv);
}

int main()
{
    {
        vector<int> Cs = {2, 1, 3, 4};
        int c = 0;
        vector<int> previous;
        choose_one_from_each_group(
            make_span(Cs.data(), Cs.size()), [&c, &Cs, &previous](auto v) {
                const int N = Cs.size();
                assert(v.size() == N);
                for (int i = 0; i < N; ++i)
                    assert(0 <= v[i] && v[i] < Cs[i]);
                if (c > 0)
                    assert(make_span(previous.data(), previous.size()) < v);
                ++c;
                previous.assign(BE(v));
            });
        assert(c == std::accumulate(BE(Cs), 1, std::multiplies<int>()));
    }
    {
        for (double x = -1; x < 1; x += 0.1) {
            assert_approx_eq(difftan<0>(x), tan(x), 1e-10);
            assert_approx_eq(
                difftan<1>(x),
                approx_deriv(1, [](double x) { return tan(x); }, x), 1e-8);
            assert_approx_eq(
                difftan<2>(x),
                approx_deriv(2, [](double x) { return tan(x); }, x), 1e-5);
            assert_approx_eq(
                difftan<3>(x),
                approx_deriv(3, [](double x) { return tan(x); }, x), 1e-3);
        }
    }
    {
        for (double x = -1; x < 1; x += 0.1) {
            assert_approx_eq(diffcos<0>(x), cos(x), 1e-10);
            assert_approx_eq(
                diffcos<1>(x),
                approx_deriv(1, [](double x) { return cos(x); }, x), 1e-8);
            assert_approx_eq(
                diffcos<2>(x),
                approx_deriv(2, [](double x) { return cos(x); }, x), 1e-5);
            assert_approx_eq(
                diffcos<3>(x),
                approx_deriv(3, [](double x) { return cos(x); }, x), 1e-3);
        }
    }
    {
        for (double x = -1; x < 1; x += 0.1) {
            assert_approx_eq(diffsin<0>(x), sin(x), 1e-10);
            assert_approx_eq(
                diffsin<1>(x),
                approx_deriv(1, [](double x) { return sin(x); }, x), 1e-8);
            assert_approx_eq(
                diffsin<2>(x),
                approx_deriv(2, [](double x) { return sin(x); }, x), 1e-5);
            assert_approx_eq(
                diffsin<3>(x),
                approx_deriv(3, [](double x) { return sin(x); }, x), 1e-3);
        }
    }
    {
        using ul::polyder;
        using ul::polyval;
        for (double a = -1; a <= 1; a += 0.1) {
            auto p0 = ul::taylor_tan<0>(a);
            auto p1 = ul::taylor_tan<1>(a);
            auto p2 = ul::taylor_tan<2>(a);
            auto p3 = ul::taylor_tan<3>(a);
            assert_approx_eq(polyval(p0, a), difftan<0>(a), 1e-10);
            assert_approx_eq(polyval(p1, a), difftan<0>(a), 1e-10);
            assert_approx_eq(polyval(p2, a), difftan<0>(a), 1e-10);
            assert_approx_eq(polyval(p3, a), difftan<0>(a), 1e-10);

            assert_approx_eq(polyval(polyder(p1), a), difftan<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p2), a), difftan<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p3), a), difftan<1>(a), 1e-10);

            assert_approx_eq(polyval(polyder(polyder(p2)), a), difftan<2>(a),
                             1e-10);
            assert_approx_eq(polyval(polyder(polyder(p3)), a), difftan<2>(a),
                             1e-10);

            assert_approx_eq(polyval(polyder(polyder(polyder(p3))), a),
                             difftan<3>(a), 1e-10);
        }
    }
    {
        using ul::polyder;
        using ul::polyval;
        for (double a = -1; a <= 1; a += 0.1) {
            auto p0 = ul::taylor_cos<0>(a);
            auto p1 = ul::taylor_cos<1>(a);
            auto p2 = ul::taylor_cos<2>(a);
            auto p3 = ul::taylor_cos<3>(a);
            assert_approx_eq(polyval(p0, a), diffcos<0>(a), 1e-10);
            assert_approx_eq(polyval(p1, a), diffcos<0>(a), 1e-10);
            assert_approx_eq(polyval(p2, a), diffcos<0>(a), 1e-10);
            assert_approx_eq(polyval(p3, a), diffcos<0>(a), 1e-10);

            assert_approx_eq(polyval(polyder(p1), a), diffcos<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p2), a), diffcos<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p3), a), diffcos<1>(a), 1e-10);

            assert_approx_eq(polyval(polyder(polyder(p2)), a), diffcos<2>(a),
                             1e-10);
            assert_approx_eq(polyval(polyder(polyder(p3)), a), diffcos<2>(a),
                             1e-10);

            assert_approx_eq(polyval(polyder(polyder(polyder(p3))), a),
                             diffcos<3>(a), 1e-10);
        }
    }
    {
        using ul::polyder;
        using ul::polyval;
        for (double a = -1; a <= 1; a += 0.1) {
            auto p0 = ul::taylor_sin<0>(a);
            auto p1 = ul::taylor_sin<1>(a);
            auto p2 = ul::taylor_sin<2>(a);
            auto p3 = ul::taylor_sin<3>(a);
            assert_approx_eq(polyval(p0, a), diffsin<0>(a), 1e-10);
            assert_approx_eq(polyval(p1, a), diffsin<0>(a), 1e-10);
            assert_approx_eq(polyval(p2, a), diffsin<0>(a), 1e-10);
            assert_approx_eq(polyval(p3, a), diffsin<0>(a), 1e-10);

            assert_approx_eq(polyval(polyder(p1), a), diffsin<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p2), a), diffsin<1>(a), 1e-10);
            assert_approx_eq(polyval(polyder(p3), a), diffsin<1>(a), 1e-10);

            assert_approx_eq(polyval(polyder(polyder(p2)), a), diffsin<2>(a),
                             1e-10);
            assert_approx_eq(polyval(polyder(polyder(p3)), a), diffsin<2>(a),
                             1e-10);

            assert_approx_eq(polyval(polyder(polyder(polyder(p3))), a),
                             diffsin<3>(a), 1e-10);
        }
    }
    {
        test_polycompose<1, 1>();
        test_polycompose<1, 2>();
        test_polycompose<1, 3>();
        test_polycompose<1, 4>();
        test_polycompose<1, 5>();
        test_polycompose<2, 1>();
        test_polycompose<2, 2>();
        test_polycompose<2, 3>();
        test_polycompose<2, 4>();
        test_polycompose<2, 5>();
        test_polycompose<3, 1>();
        test_polycompose<3, 2>();
        test_polycompose<3, 3>();
        test_polycompose<3, 4>();
        test_polycompose<3, 5>();
        test_polycompose<4, 1>();
        test_polycompose<4, 2>();
        test_polycompose<4, 3>();
        test_polycompose<4, 4>();
        test_polycompose<4, 5>();
        test_polycompose<5, 1>();
        test_polycompose<5, 2>();
        test_polycompose<5, 3>();
        test_polycompose<5, 4>();
        test_polycompose<5, 5>();
    }
    printf("Done.\n");
    return 0;
}
