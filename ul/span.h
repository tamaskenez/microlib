#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

// minimal implementation (just what was needed) of the 1-D span concept,
// that is: span encapsulates a contiguous memory block

namespace ul {

template <typename T>
class span
{
public:
    using this_type = span<T>;
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using iterator = T*;
    using const_iterator = const T*;

    span() = default;

    // fundamental constructor
    span(pointer d, size_type s) : d(d), s(s) { assert(d || s == 0); }
    span(pointer d, pointer e) : d(d), s(e - d) { assert(d <= e); }
    // assignent from span
    template <typename U,
              typename = std::enable_if_t<std::is_convertible<U, T>::value>>
    this_type operator=(const span<U>& x)
    {
        d = x.data();
        s = x.size();
        return *this;
    }

    // assigment from other types
    template <typename U,
              typename = std::enable_if_t<std::is_convertible<U, T>::value>>
    this_type operator=(std::vector<U>& x)
    {
        d = x.data();
        s = x.size();
        return *this;
    }

    template <typename U,
              typename = std::enable_if_t<std::is_convertible<U, T>::value>>
    this_type operator=(const std::vector<U>& x)
    {
        d = x.data();
        s = x.size();
        return *this;
    }

    // observers
    pointer data() const { return d; }
    size_type size() const { return s; }
    bool empty() const { return s == 0; }
    // comparison
    template <typename U>
    bool operator==(span<U> y) const
    {
        const bool same_type =
            std::is_same<std::remove_cv_t<U>, std::remove_cv_t<T>>::value;
        return s == y.s &&
               (s == 0 || (same_type && d == y.d) || std::equal(d, d + s, y.d));
    }

    value_type& operator[](size_type x) const
    {
        assert(0 <= x && x < s);
        return d[x];
    }

    iterator begin() const { return d; }
    iterator end() const { return d + s; }

private:
    pointer d = nullptr;
    size_type s = 0;
};

// convenience free function constructors
template <typename T>
span<T> make_span(T* p, std::size_t s)
{
    return span<T>(p, s);
}

using cspan = span<const char>;

inline cspan as_span(const char* s)
{
    return cspan(s, strlen(s));
}
inline cspan as_span(const std::string& s)
{
    return cspan(s.data(), s.size());
}

template <class T>
span<std::add_const_t<T>> as_span(const std::vector<T>& x)
{
    return make_span(x.data(), x.size());
}

template <class T, size_t N>
span<std::add_const_t<T>> as_span(const std::array<T, N>& x)
{
    return make_span(x.data(), N);
}

template <class T>
bool operator<(span<T> x, span<T> y)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class T>
typename span<T>::iterator begin(const span<T>& x)
{
    return x.begin();
}

template <class T>
typename span<T>::iterator end(const span<T>& x)
{
    return x.end();
}

template <class T>
std::string to_string(span<T> x)
{
    using std::to_string;
    bool first = true;
    std::string s("{");
    for (auto it = x.begin(); it != x.end(); ++it) {
        if (!first)
            s += ", ";
        else
            first = false;
        s += to_string(*it);
    }
    s += "}";
    return s;
}

}  // namespace ul
