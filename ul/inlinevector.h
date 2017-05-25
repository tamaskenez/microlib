#pragma once

#include <array>
#include <cassert>

#include "ul/type_traits.h"

namespace ul {

using std::array;

// cheap inlinevector built around std::array
// suitable mostly for trivial types since all items are constructed/destructed
// with the object
template <class T, int Capacity>
class InlineVector
{
public:
    using array_t = array<T, Capacity>;
    using iterator = typename array_t::iterator;
    using const_iterator = typename array_t::const_iterator;

    int size() const { return s; }
    constexpr int capacity() const { return Capacity; }
    bool empty() const { return s == 0; }
    T& operator[](int x) { return a[x]; }
    const T& operator[](int x) const { return a[x]; }
    T& front()
    {
        assert(s > 0);
        return a[0];
    }
    const T& front() const
    {
        assert(s > 0);
        return a[0];
    }
    T& back()
    {
        assert(s > 0);
        return a[s - 1];
    }
    const T& back() const
    {
        assert(s > 0);
        return a[s - 1];
    }

    iterator begin() { return a.begin(); }
    iterator end() { return a.begin() + s; }
    const_iterator begin() const { return a.begin(); }
    const_iterator end() const { return a.begin() + s; }
    void push_back(const T& x)
    {
        assert(s < Capacity);
        a[s++] = x;
    }
    void pop_back()
    {
        assert(s > 0);
        --s;
    }
    void erase(const_iterator it)
    {
        int idx = it - a.begin();
        assert(0 <= idx && idx < s);
        for (int i = idx; i + 1 < s; ++i)
            a[i] = std::move(a[i + 1]);
        --s;
    }

    void clear() { s = 0; }
    void resize(int i)
    {
        assert(0 <= i && i <= Capacity);
        s = i;
    }

private:
    int s = 0;
    array_t a;
};

template <class T, int N>
struct range_code<InlineVector<T, N>> : std::integral_constant<ptrdiff_t, N>
{
};
}
