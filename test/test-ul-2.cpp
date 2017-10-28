#undef NDEBUG

#include <cassert>

#include "ul/ul.h"

void scope_exit_test2()
{
    printf("scope_exit_test2.\n");
    int b = 2;
    {
        assert(b == 2);
        ON_SCOPE_EXIT { b += 10; };
        assert(b == 2);
    }
    assert(b == 12);
}
