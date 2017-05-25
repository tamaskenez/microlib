#pragma once

#include <bitset>

namespace ul {

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
}
