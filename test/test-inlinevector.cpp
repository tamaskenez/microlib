#undef NDEBUG

#include <cassert>

#include "ul/inlinevector.h"
#include "ul/ul.h"

#include "test_common.cpp"

template <class X,
          class Y,
          UL_T_ENABLE_IF(
              ul::range_code<X>::value >= ul::c_range_code_sized_iterable &&
              ul::range_code<Y>::value >= ul::c_range_code_sized_iterable)>
bool operator==(const X& x, const Y& y)
{
    if (x.size() != y.size())
        return false;
    return std::equal(x.begin(), x.end(), y.begin());
}

int main()
{
    const int N = 10;
    {
        ul::InlineVector<int, N> a;
        assert(std::all_of(BE(a), [](auto x) { return x == 0; }));
    }
    {
        ul::InlineVector<int, N> a(ul::uninitialized);
        a.resize(N, 2);
        assert(std::all_of(BE(a), [](auto x) { return x == 2; }));
        a.resize(0);
        std::fill(a.data(), a.data() + N, 3);
        a.resize(N, ul::uninitialized);
        assert(std::all_of(BE(a), [](auto x) { return x == 3; }));
    }
    ul::InlineVector<int, N> iv;
    std::vector<int> v;

    for (int i = 0; i < N; ++i) {
        iv.push_back(i);
        v.push_back(i);
        assert(v == iv);
    }

    v.erase(v.begin());
    iv.erase(iv.begin());
    assert(v == iv);

    v.erase(v.begin() + v.size() - 1);
    iv.erase(iv.begin() + iv.size() - 1);
    assert(v == iv);

    v.erase(v.begin() + 1);
    iv.erase(iv.begin() + 1);
    assert(v == iv);

    while (!v.empty())
        v.pop_back();

    iv.clear();
    iv.push_back(10);
    v.push_back(10);
    assert(v == iv);

    printf("Done.\n");
}
