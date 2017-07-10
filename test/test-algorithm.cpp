#include <algorithm>
#include <cassert>
#include <vector>

#include "ul/algorithm.h"
#include "ul/ul.h"

int main()
{
    std::vector<int> v = {5, 3, 3, 1};
    auto sv = v;
    sort(BE(sv));

    assert(ul::binary_find(sv, 4) == sv.end());
    assert(!ul::contains(sv, 4));
    for (auto x : sv) {
        assert(*ul::binary_find(sv, x) == x);
        assert(ul::contains(sv, x));
    }

    auto sutv = v;
    ul::sort_unique_trunc(sutv);
    std::vector<int> r = {1, 3, 5};
    assert(sutv == r);

    printf("Done.\n");
}
