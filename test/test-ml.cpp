#undef NDEBUG

#include <algorithm>
#include <cassert>
#include <climits>
#include <numeric>

#include "ul/ml.h"
#include "ul/span.h"
#include "ul/ul.h"

using std::array;
using ul::make_span;
using ul::ml::polyval;

int main()
{
    array<int, 5> c{{2, -3, -4, 5, 6}};
    for (int x = -3; x <= 3; ++x) {
        auto ya = polyval(make_span(c.data(), 1), x);
        auto yb = c[0];
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 2), x);
        yb += c[1] * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 3), x);
        yb += c[2] * x * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 4), x);
        yb += c[3] * x * x * x;
        assert(ya == yb);

        ya = polyval(make_span(c.data(), 5), x);
        yb += c[4] * x * x * x * x;
        assert(ya == yb);
    }
    printf("Done.\n");
    return 0;
}
