#undef NDEBUG
#include "ul/inlinevector.h"
#include "ul/ul.h"

using namespace ul;

template <class X,
          class Y,
          UL_T_ENABLE_IF(range_code<X>::value >= c_range_code_sized_iterable &&
                         range_code<Y>::value >= c_range_code_sized_iterable)>
bool operator==(const X& x, const Y& y)
{
    if (x.size() != y.size())
        return false;
    return std::equal(x.begin(), x.end(), y.begin());
}

int main()
{
    const int N = 10;
    InlineVector<int, N> iv;
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
}
