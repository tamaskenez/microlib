#pragma once

#include <cstring>
#include <string>

namespace ul {

using std::string;

// string_par can be used in function where both const char* and
// const std::string& can be accepted:
//
// void foo(string_par s) {
//     ... = s.c_str();
// }
//
// foo("bar");
// std::string s;
// foo(s);
//
class string_par
{
public:
    using iterator = const char*;
    using const_iterator = iterator;

    string_par() = delete;

    string_par(const string_par&) = default;

    string_par(const char* s) : s(s), size_(string::npos) {}
    string_par(const string& s) : s(s.c_str()), size_(s.size()) {}

    const char* c_str() const { return s; }
    operator const char*() const { return s; }
    string str() const { return s; }
    bool empty() const { return !s || *s == 0; }
    size_t size() const
    {
        if (size_ == string::npos) {
            auto mutable_this = const_cast<string_par*>(this);
            mutable_this->size_ = strlen(s);
        }
        return size_;
    }

    iterator begin() const { return s; }
    iterator end() const { return s + size(); }

private:
    const char* s;
    size_t size_;
};

inline bool operator==(string_par x, const string& y)
{
    return y == x.c_str();
}
inline bool operator==(const string& y, string_par x)
{
    return y == x.c_str();
}
inline bool operator==(string_par x, const char* y)
{
    return strcmp(y, x.c_str()) == 0;
}
inline bool operator==(const char* y, string_par x)
{
    return strcmp(y, x.c_str()) == 0;
}
inline bool operator!=(string_par x, const string& y)
{
    return y != x.c_str();
}
inline bool operator!=(const string& y, string_par x)
{
    return y != x.c_str();
}
inline bool operator!=(string_par x, const char* y)
{
    return strcmp(y, x.c_str()) != 0;
}
inline bool operator!=(const char* y, string_par x)
{
    return strcmp(y, x.c_str()) != 0;
}
}  // namespace ul
