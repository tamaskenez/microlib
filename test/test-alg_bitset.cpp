#undef NDEBUG

#include <cassert>

#include "ul/alg_bitset.h"

#include "test_common.cpp"

template <size_t N>
void f(const std::bitset<N>& bs)
{
    assert(ULCC any_of(bs) == bs.any());
    assert(ULCC all_of(bs) == bs.all());
    assert(ULCC none_of(bs) == bs.none());
}

int main()
{
    std::bitset<2> bfalse;
    std::bitset<2> bmixed{"01"};
    std::bitset<2> btrue{"11"};

    f(bfalse);
    f(bmixed);
    f(btrue);
}
