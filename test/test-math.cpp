#undef NDEBUG

#include <cassert>
#include <cfloat>
#include <climits>
#include <numeric>

#include "ul/math.h"
#include "ul/ul.h"

using std::vector;
using ul::make_span;
using ul::sec;
using ul::sign;
using ul::square;
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

    assert(square(3) == 9);
    assert(square(-3) == 9);
    assert(square(3.0) == 9.0);
    assert(square(-3.0) == 9.0);

    assert(sec(0.0) == 1.0);
    assert(fabs(sec(M_PI) + 1) < 1e3 * DBL_MIN);
    assert(fabs(sec(.1) - (1 / cos(.1))) < 1e3 * DBL_MIN);
    assert(fabs(sec(-.1) - (1 / cos(.1))) < 1e3 * DBL_MIN);

    printf("Done.\n");
    return 0;
}
