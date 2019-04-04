#include "simple_test.hpp"
#include "ul/string_par.h"

namespace ul {
void f1(string_par sp, const std::string& ss, bool b = true)
{
    CHECK(ss == sp.c_str());
    CHECK(ss == (const char*)sp);
    CHECK(ss == sp.str());
    string abd("abd");
    CHECK(sp == ss);
    CHECK(!(sp == abd));
    CHECK(ss == sp);
    CHECK(!(abd == sp));
    CHECK(!(sp != ss));
    CHECK(sp != abd);
    CHECK(!(ss != sp));
    CHECK(abd != sp);
    if (b)
        f1(sp, ss, false);
}

void f2(string_par sp, const char* cc, bool b = true)
{
    CHECK(strcmp(cc, sp.c_str()) == 0);
    CHECK(strcmp(cc, (const char*)sp) == 0);
    CHECK(sp.str() == cc);
    const char* abd = "abd";
    CHECK(sp == cc);
    CHECK(!(sp == abd));
    CHECK(cc == sp);
    CHECK(!(abd == sp));
    CHECK(!(sp != cc));
    CHECK(sp != abd);
    CHECK(!(cc != sp));
    CHECK(abd != sp);
    if (b)
        f2(sp, cc, false);
}

int main(int, const char*[])
{
    string_par s1(nullptr);
    CHECK(s1.empty());
    string_par s2("");
    CHECK(s2.empty());
    string e;
    string_par s3(e);
    CHECK(s3.empty());

    f1("", "");
    f1("abc", "abc");
    f2("", "");
    f2("abc", "abc");

    string abc("abc");

    f1(e, "");
    f1(abc, "abc");
    f2(e, "");
    f2(abc, "abc");

    return test_result();
}
}  // namespace ul

int main(int argc, const char* argv[])
{
    return ul::main(argc, argv);
}
