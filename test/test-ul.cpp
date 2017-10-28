#undef NDEBUG

#include <cassert>
#include <vector>

#include "ul/ul.h"

using std::vector;
using ul::scope_exit_task;

void scope_exit_test2();

template <class A, class B, class C, class D>
void f(A a, B b, C c, D d)
{
    assert(a == c);
    assert(b == d);
}

void for_test()
{
    vector<int> v(10, 0);
    f(BE(v), v.begin(), v.end());

    int c;

    c = 0;
    FOR(i, 10, < 13)
    {
        assert(10 <= i && i < 13);
        ++c;
    }
    assert(c == 3);

    c = 0;
    FOR(i, 10, != 13)
    {
        assert(10 <= i && i < 13);
        ++c;
    }
    assert(c == 3);

    c = 0;
    FOR(i, 10, <= 13)
    {
        assert(10 <= i && i <= 13);
        ++c;
    }
    assert(c == 4);

    c = 0;
    FORBACK(i, 13, >= 10)
    {
        assert(10 <= i && i <= 13);
        ++c;
    }
    assert(c == 4);

    c = 0;
    FORBE(it, v)
    {
        assert(v.begin() <= it && it < v.end());
        ++c;
    }
    assert(c == 10);
}

void scope_exit_test()
{
    printf("scope_exit_test.\n");
    int b = 123;
    {
        assert(b == 123);
        ON_SCOPE_EXIT { b *= 2; };
        assert(b == 123);
    }
    assert(b == 246);

    int c = 2;
    int d = 3;
    int e = 4;
    int f = 5;
    int g = 6;
    {
        scope_exit_task t1;
        scope_exit_task t2;
        scope_exit_task t3([&d]() { d += 10; });
        scope_exit_task t4([&e]() { e += 10; });
        scope_exit_task t5([&f]() { f += 10; });
        scope_exit_task t6([&g]() { ++g; });
        scope_exit_task t7(std::move(t6));

        t2 = [&c]() { c = 12; };
        t4 = ([&e]() { e = 114; });
        t5 = nullptr;
        assert(c == 2);
        assert(d == 3);
        assert(e == 4);
        assert(f == 5);
        assert(g == 6);
    }
    assert(c == 12);
    assert(d == 13);
    assert(e == 114);
    assert(f == 5);
    assert(g == 7);
}

int main()
{
    for_test();
    scope_exit_test();
    scope_exit_test2();

    printf("Done.\n");
    return 0;
}
