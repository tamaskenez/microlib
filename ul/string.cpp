#include "ul/string.h"
#include "ul/ul.h"

namespace ul {
bool startswith(string_par s, string_par prefix)
{
    if (s.size() < prefix.size())
        return false;
    if (prefix.size() == 0)
        return true;
    FOR(i, 0, < prefix.size()) if (s[i] != prefix[i]) return false;
    return true;
}
}  // namespace ul
