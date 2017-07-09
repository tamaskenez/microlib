#undef NDEBUG

#include <cassert>

#include "ul/math.h"

using ul::within_co;

int main()
{
    assert(!within_co(-1, 0, 10));
    assert(within_co(0, 0, 10));
    assert(within_co(1, 0, 10));
    assert(within_co(9, 0, 10));
    assert(!within_co(10, 0, 10));
    assert(!within_co(11, 0, 10));
    return 0;
}
