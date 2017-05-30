#undef NDEBUG

#include <cassert>
#include <vector>

#include "ul/ul.h"

#include "test_common.cpp"

using std::vector;

template <class A, class B, class C, class D>
void f(A a, B b, C c, D d)
{
    assert(a == c);
    assert(b == d);
}

int main()
{
    vector<int> v(10, 0);
    f(BE(v), v.begin(), v.end());

    int c;

    c = 0;
    FOR(i, 10, 13)
    {
        assert(10 <= i && i < 13);
        ++c;
    }
    assert(c == 3);

    c = 0;
    FORNEQ(i, 10, 13)
    {
        assert(10 <= i && i < 13);
        ++c;
    }
    assert(c == 3);

    c = 0;
    FORLEQ(i, 10, 13)
    {
        assert(10 <= i && i <= 13);
        ++c;
    }
    assert(c == 4);

    c = 0;
    FORGEQ(i, 13, 10)
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

    printf("Done.\n");
    return 0;
}
