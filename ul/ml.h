#pragma once

#include "ul/span.h"

#include <type_traits>
#include <utility>

#include "ul/type_traits.h"

// matlab-like functions
namespace ul {
namespace ml {
template <class V, class U>
auto polyval(const V& p, U x) -> decltype(*begin(p) * U(0))
{
    using R = decltype(*begin(p) * U(0));
    switch (p.size()) {
        case 0:
            return 0;
        case 1:
            return p[0];
        case 2:
            return p[0] + x * p[1];
        default:
            R xa = x;
            R acc = p[0] + p[1] * x;
            for (int i = 2; i < p.size(); ++i) {
                xa *= x;
                acc += p[i] * xa;
            }
            return acc;
    }
}

template <class V>
auto polyder(const V& p)
{
    auto result = ul::make_array_or_vector_of_given_size<
        typename ul::value_type<V>::type,
        ul::size_of_fixed_size_range_or_zero<V>::value - 1,
        ul::is_fixed_size_range<V>::value>(p.size() - 1);
    for (int i = 0; i + 1 < p.size(); ++i) {
        result[i] = (i + 1) * p[i + 1];
    }
    return result;
}

}  // namespace ml
}  // namespace ul
