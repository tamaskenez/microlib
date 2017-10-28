#undef NDEBUG

#include <cassert>
#include <climits>

#include "ul/math.h"

using ul::sign;
using ul::within_co;

int main()
{
    assert(!within_co(-1, 0, 10));
    assert(within_co(0, 0, 10));
    assert(within_co(1, 0, 10));
    assert(within_co(9, 0, 10));
    assert(!within_co(10, 0, 10));
    assert(!within_co(11, 0, 10));

    assert(sign(INT_MIN) == -1);
    assert(sign(-2) == -1);
    assert(sign(-1) == -1);
    assert(sign(0) == 0);
    assert(sign(1) == 1);
    assert(sign(2) == 1);
    assert(sign(INT_MAX) == 1);
    assert(sign(UINT_MAX) == 1);
    assert(sign(-INFINITY) == -1);
    assert(sign(-2.3456789) == -1);
    assert(sign(-1.0) == -1);
    assert(sign(-1e-200) == -1);
    assert(sign(0) == 0);
    assert(sign(1e-200) == 1);
    assert(sign(1.0) == 1);
    assert(sign(2.3456789) == 1);
    assert(sign(INFINITY) == 1);
    return 0;
}
