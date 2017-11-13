#undef NDEBUG

#include "ul/math.h"
#include "ul/stopwatch.h"

#include <cassert>
#include <cmath>
#include <thread>

using std::chrono::seconds;
using std::this_thread::sleep_for;

using ul::Stopwatch;
using ul::tic;
using ul::toc;

using std::chrono::milliseconds;
using std::this_thread::sleep_for;

const milliseconds waitms(100);
const double waitsec(.1);
const double tol(.03);

template <class T, class U, class V>
void assert_approx_eq(T t, U u, V v)
{
    assert(fabs(t - u) <= v);
}

void test_tictoc()
{
    double e0 = 0.0, e1 = 0.0;
    auto t1 = tic();
    auto t0 = tic();
    sleep_for(seconds(1));
    e0 += toc(t0);
    t0 = tic();
    sleep_for(seconds(1));
    e0 += toc(t0);
    e1 += toc(t1);
    assert_approx_eq(e0, 2.0, tol);
    assert_approx_eq(e1, 2.0, tol);
    printf("%f\n", e0);
    printf("%f\n", e1);
}

void test_ctor_default()
{
    Stopwatch<> sw;
    assert(!sw.running());
    assert(sw.elapsed() == 0.0);
    sw.start();
    assert(sw.running());
    sleep_for(waitms);
    assert_approx_eq(sw.elapsed(), waitsec, tol);
    sleep_for(waitms);
    double e1 = sw.stop();
    assert_approx_eq(e1, 2 * waitsec, tol);
    assert_approx_eq(sw.elapsed(), 2 * waitsec, tol);
    assert(!sw.running());
}

void test_ctor_true()
{
    Stopwatch<> sw(true);
    assert(sw.running());
    sleep_for(waitms);
    assert_approx_eq(sw.elapsed(), waitsec, tol);
    sleep_for(waitms);
    double e1 = sw.stop();
    assert_approx_eq(e1, 2 * waitsec, tol);
    assert_approx_eq(sw.elapsed(), 2 * waitsec, tol);
    assert(!sw.running());
    sw.start();
    assert(sw.running());
    sleep_for(waitms);
    double e2 = sw.stop();
    assert_approx_eq(e2, 3 * waitsec, tol);
    assert(ul::is_zero(sw.elapsed() - e2));
    assert(!sw.running());
    double e3 = sw.restart();
    assert(ul::is_zero(e2 - e3));
    assert(sw.running());
    sleep_for(waitms);
    double e4 = sw.elapsed();
    double e5 = sw.stop();
    assert_approx_eq(e4, waitsec, tol);
    assert_approx_eq(e5, waitsec, tol);
    assert(!sw.running());
    sw.start();
    assert(sw.running());
    sw.reset();
    assert(!sw.running());
    assert(sw.elapsed() == 0.0);
}

int main()
{
    test_tictoc();
    test_ctor_default();
    test_ctor_true();
    printf("Done.\n");
    return 0;
}
