#pragma once

#include <string>

#include "ul/type_traits.h"

namespace ul {

using std::string;

template <class It>
string to_string_be(It b, It e);

template <class T,
          UL_T_ENABLE_IF(range_code<T>::value >= c_range_code_iterable)>
string to_string(const T& a)
{
    return to_string_be(a.begin(), a.end());
}

template <class It>
string to_string_be(It b, It e)
{
    using value_type = typename std::decay<decltype(*b)>::type;
    constexpr bool is_string = std::is_same<value_type, std::string>::value;
    string s;
    if (b == e)
        s = "{}";
    else {
        using std::to_string;
        s = is_string ? "{\"" : "{";
        bool first = true;
        for (auto it = b; it != e; ++it) {
            if (first)
                first = false;
            else
                s += is_string ? "\", \"" : ", ";
            s += to_string(*it);
        }
        s += is_string ? "\"}" : "}";
    }
    return s;
}
}  // namespace ul
