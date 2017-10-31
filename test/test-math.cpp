#undef NDEBUG

#include <cassert>
#include <climits>
#include <numeric>

#include "ul/math.h"
#include "ul/ul.h"

using ul::make_span;
using ul::sign;
using ul::within_co;

using std::vector;

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

    {
        vector<int> Cs = {2, 1, 3, 4};
        int c = 0;
        vector<int> previous;
        choose_one_from_each_group(
            make_span(Cs.data(), Cs.size()), [&c, &Cs, &previous](auto v) {
                const int N = Cs.size();
                assert(v.size() == N);
                for (int i = 0; i < N; ++i)
                    assert(0 <= v[i] && v[i] < Cs[i]);
                if (c > 0)
                    assert(make_span(previous.data(), previous.size()) < v);
                ++c;
                previous.assign(BE(v));
            });
        assert(c == std::accumulate(BE(Cs), 1, std::multiplies<int>()));
    }
    printf("Done.\n");
    return 0;
}
