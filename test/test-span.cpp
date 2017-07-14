#include <array>

#include "ul/span.h"
#include "simple_test.hpp"

namespace ul {

using ul::span;
using std::array;

int main(int, const char* [])
{
    span<int> a;
    CHECK(a.empty() == true);
    CHECK(a.size() == static_cast<size_t>(0));
    CHECK(a.begin() == a.end());

    array<int, 3> ab = {{1, 2, 3}};

    span<int> b1(ab.data(), ab.size());

    CHECK(b1.begin() < b1.end());

    CHECK(b1.size() == ab.size());
    for (size_t i = 0; i < ab.size(); ++i) {
        CHECK(ab[i] == b1[i]);
    }

    int i = 0;
    for (auto& x : b1) {
        CHECK(x == ab[i++]);
    }

    return test_result();
}
}
int main(int argc, const char* argv[])
{
    return ul::main(argc, argv);
}
