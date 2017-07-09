#pragma once

#include <cassert>

namespace ul {
template <class X, class L, class U>
bool within_co(const X& x, const L& lower, const U& upper)
{
    assert(lower <= upper);
    return lower <= x && x < upper;
}
}