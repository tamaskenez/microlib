#include "ul/string.h"
#include "ul/ul.h"

namespace ul {

using std::vector;

bool startswith(string_par s, string_par prefix)
{
    if (s.size() < prefix.size()) {
        return false;
    }
    if (prefix.size() == 0) {
        return true;
    }
    FOR(i, 0, < prefix.size())
    {
        if (s[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

bool startswith(string_par s, char prefix)
{
    return !s.empty() && s[0] == prefix;
}

bool endswith(string_par s, string_par prefix)
{
    if (s.size() < prefix.size()) {
        return false;
    }
    if (prefix.empty()) {
        return true;
    }
    auto k = s.size() - prefix.size();
    FOR(i, 0, < prefix.size())
    {
        if (s[k + i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

span<const char> trim(span<const char> s)
{
    auto b = s.begin();
    auto e = s.end();
    while (b < e && isspace(*b)) {
        ++b;
    }
    if (b == e) {
        return {};
    }
    for (;;) {
        --e;
        if (!isspace(*e)) {
            ++e;
            break;
        }
    }
    return {b, e};
}

void split(span<const char> s,
           const char* separators,
           std::vector<span<const char>>& result)
{
    result.clear();

    auto b = s.begin();
    auto e = s.end();
    for (;;) {
        if (b == e) {
            break;
        }
        if (strchr(separators, *b)) {
            result.emplace_back();
            ++b;
            continue;
        }
        auto m = b + 1;
        while (m < e && !strchr(separators, *m)) {
            ++m;
        }
        result.emplace_back(b, m);
        if (m == e) {
            break;
        }
        assert(strchr(separators, *m));
        b = m + 1;
    }
}
}  // namespace ul
