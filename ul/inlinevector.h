#pragma once

#include <array>
#include <cassert>

#include "ul/check.h"
#include "ul/type_traits.h"
#include "ul/ul.h"

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

    // Default constructor aggregate-initializes underlying array
    InlineVector() : a{} {}

    explicit InlineVector(uninitialized_t) {}

    InlineVector(int n, uninitialized_t) : s(n) { CHECK(n <= Capacity); }

    explicit InlineVector(int n, const T& x) : s(n)
    {
        CHECK(n <= Capacity);
        for (int i = 0; i < n; ++i)
            a[i] = x;
    }

    explicit InlineVector(std::initializer_list<T> x) : s(x.size())
    {
        CHECK(x.size() <= Capacity);
        std::copy(BE(x), a.begin());
    }

    template <class U, size_t N>
    void operator=(const std::array<U, N>& x)
    {
        CHECK(x.size() <= Capacity);
        std::copy(BE(x), a.begin());
        s = x.size();
    }

    int size() const { return s; }
    constexpr int capacity() const { return Capacity; }
    bool empty() const { return s == 0; }
    T& operator[](int x)
    {
        assert(0 <= x && x < s);
        return a[x];
    }
    const T& operator[](int x) const
    {
        assert(0 <= x && x < s);
        return a[x];
    }
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
        CHECK(0 <= idx && idx < s);
        for (int i = idx; i + 1 < s; ++i)
            a[i] = std::move(a[i + 1]);
        --s;
    }

    void clear() { s = 0; }

    void resize(int i, uninitialized_t)
    {
        CHECK(0 <= i && i <= Capacity);
        s = i;
    }

    void resize(int i, const T& value = T())
    {
        CHECK(0 <= i && i <= Capacity);
        for (int j = s; j < i; ++j)
            a[j] = value;
        s = i;
    }

    auto data() { return a.data(); }
    auto data() const { return a.data(); }

private:
    int s = 0;
    array_t a;
};

template <class T, int N>
struct range_code<InlineVector<T, N>> : std::integral_constant<ptrdiff_t, N>
{
};

template <class T, int N>
struct is_resizable<InlineVector<T, N>> : std::true_type
{
};

}  // namespace ul
