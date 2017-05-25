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
    printf("Done.\n");
    return 0;
}
