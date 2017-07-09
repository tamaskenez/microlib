#undef NDEBUG

#include <cassert>

#include "ul/string.h"

using ul::startswith;

int main()
{
    assert(startswith("", ""));
    assert(startswith("qwe", ""));
    assert(startswith("qwe", "q"));
    assert(startswith("qwe", "qw"));
    assert(startswith("qwe", "qwe"));
    assert(!startswith("qwe", "w"));
    assert(!startswith("qwe", "1w"));
    assert(!startswith("qwe", "q1"));
    assert(!startswith("qwe", "qw1"));
    return 0;
}
