#include "ul/stringf.h"

#include <cerrno>
#include <cstdarg>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

namespace ul {
const size_t STRINGF_BUFSIZE = 1024;

#ifndef _MSC_VER
#define SX_VSNPRINTF vsnprintf
#else
#define SX_VSNPRINTF _vsnprintf
#endif

std::string stringf(const char* fmt, ...)
{
    char buf[STRINGF_BUFSIZE];
    va_list ap;
    va_start(ap, fmt);
    int r = SX_VSNPRINTF(buf, STRINGF_BUFSIZE, fmt, ap);
    va_end(ap);

    if (0 <= r && static_cast<size_t>(r) < STRINGF_BUFSIZE) {
        return buf;
    }

    errno = 0;

#ifdef _MSC_VER
    if (r < 0 && r != c_stringf_bufsize) {
        va_start(ap, fmt);
        r = SX_VSNPRINTF(nullptr, 0, fmt, ap);
        va_end(ap);
    }
#endif

    if (r < 0)
        throw std::runtime_error(
            "vsnprintf returned negative number for size query (buffer and "
            "size are NULL)");

    std::vector<char> buf2(r + 1);
    buf2[0] = 0;
    va_start(ap, fmt);
    SX_VSNPRINTF(buf2.data(), r + 1, fmt, ap);
    va_end(ap);
    return buf2.data();
}

std::string vstringf(const char* fmt, va_list args)
{
    char buf[STRINGF_BUFSIZE];
    va_list args_copy;
    va_copy(args_copy, args);
    int r = SX_VSNPRINTF(buf, STRINGF_BUFSIZE, fmt, args_copy);
    va_end(args_copy);

    if (0 <= r && static_cast<size_t>(r) < STRINGF_BUFSIZE) {
        return buf;
    }

    errno = 0;

#ifdef _MSC_VER
    if (r < 0 && r != c_stringf_bufsize) {
        va_copy(args_copy, args);
        r = SX_VSNPRINTF(nullptr, 0, fmt, args);
        va_end(args_copy);
    }
#endif

    if (r < 0)
        throw std::runtime_error(
            "vsnprintf returned negative number for size query (buffer and "
            "size are NULL)");

    std::vector<char> buf2(r + 1);
    buf2[0] = 0;

    va_copy(args_copy, args);
    SX_VSNPRINTF(buf2.data(), r + 1, fmt, args);
    va_end(args_copy);

    return buf2.data();
}
}  // namespace ul
