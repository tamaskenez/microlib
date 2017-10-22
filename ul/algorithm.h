#pragma once

#include <algorithm>

#include "ul/ul.h"

namespace ul {

template <class Iter, class T>
Iter binary_find(Iter first, Iter last, const T& val)
{
    auto it = std::lower_bound(first, last, val);

    if (it != last && !(val < *it))
        return it;  // found
    else
        return last;  // not found
}

template <class Range, class T>
typename Range::const_iterator binary_find(const Range& rng, const T& val)
{
    return binary_find(BE(rng), val);
}

template <class Iter, class T>
bool contains(Iter first, Iter last, const T& val)
{
    auto it = std::find(first, last, val);
    return it != last;
}

template <class Range, class T>
bool contains(const Range& rng, const T& val)
{
    return contains(BE(rng), val);
}

template <class Cont>
void unique_trunc(Cont& cont)
{
    cont.erase(std::unique(BE(cont)), cont.end());
}

template <class Cont>
void sort_unique_trunc(Cont& cont)
{
    std::sort(BE(cont));
    unique_trunc(cont);
}
}  // namespace ul
