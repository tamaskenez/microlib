#pragma once

#include <bitset>

/*

These functions:

        any_of(const bitset<N>& X)
        all_of(const bitset<N>& X)
        none_of(const bitset<N>& X)

apply the predicate on the bitset and allow expressions like

    if (all_of(EW(some_array) > 0))

where the EW macro means element-wise operations.

*/

namespace ul {

using std::bitset;

template <size_t N>
bool any_of(const bitset<N>& bs)
{
    return bs.any();
}

template <size_t N>
bool all_of(const bitset<N>& bs)
{
    return bs.all();
}

template <size_t N>
bool none_of(const bitset<N>& bs)
{
    return bs.none();
}
}
