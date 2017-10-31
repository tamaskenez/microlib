#pragma once

#include <cassert>
#include <cmath>
#include <vector>

#include "ul/span.h"

namespace ul {

template <class X, class L, class U>
bool within_co(const X& x, const L& lower, const U& upper)
{
    assert(lower <= upper);
    return lower <= x && x < upper;
}

template <class T>
constexpr int sign(T x) noexcept
{
    return x == T(0) ? 0 : (std::signbit(x) ? -1 : 1);
}

// There are N groups with Ci choices (0 <= i < N). Select a choice from
// each group 0 <= ci < Ci. Enumerate all possibilites.
// The function `f` will be called with `span<I>` of length N, containing a
// possible set of choices.
// `I` is expected to be an integral type.
// Possibilities will be enumerated in lexicographic order.
// All items in Cs must be greater than zero.
template <class F, class I>
void choose_one_from_each_group(span<I> Cs, F&& f)
{
    if (Cs.empty())
        return;
    assert(std::all_of(Cs.begin(), Cs.end(), [](auto i) { return i > 0; }));
    std::vector<std::decay_t<I>> v(Cs.size(), I(0));
    span<I> span_out(v.data(), v.size());
    for (;;) {
        f(span_out);
        int i = v.size() - 1;
        while (++v[i] >= Cs[i]) {
            if (i == 0)
                return;
            v[i--] = I(0);
        }
    }
}

}  // namespace ul
