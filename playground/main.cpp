#include <functional>
#include <type_traits>
#include <array>

#if 0
using std::forward;

using AI1 = std::array<int, 2>;

AI1 operator+(const AI1& x, const AI1& y)
{
    return AI1{{x[0] + y[0]}};
}

template <class X, class Y, class Fun>
void eval1(Fun fun, X&& x, Y&& y)
{
    fun(x, y);
}

template <class X, class Y>
auto myplus(const X& x, const Y& y) -> decltype(x + y)
{
    return x + y;
}
#endif
int main()
{
#if 0
    AI1 x, y;

    operator+(x, y);
    std::plus<>{}(x, y);
    myplus(x, y);
#endif
    return 0;
}
