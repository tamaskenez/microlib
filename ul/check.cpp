#include "ul/check.h"

#include <cstdarg>
#include <cstdio>
#include <exception>

namespace ul {
namespace detail {

void check_failed_core(const char* condition,
                       const char* file,
                       int line,
                       const char* function)
{
    fprintf(stderr, "CHECK failed: (%s) in `%s` @ %s:%d.\n", condition,
            function, file, line);
    std::terminate();
}

void check_failed_core(const char* condition,
                       const char* file,
                       int line,
                       const char* function,
                       const char* format_string,
                       ...)
{
    const int c_buf_size = 4096;
    char message[c_buf_size];
    va_list args;
    va_start(args, format_string);
    int len = vsnprintf(message, c_buf_size, format_string, args);
    va_end(args);
    if (len >= c_buf_size) {
        // replace end with '...'
        for (int i = c_buf_size - 4; i < c_buf_size - 1; ++i)
            message[i] = '.';
    }

    fprintf(stderr, "CHECK failed \"%s\": (%s) in `%s` @ %s:%d.\n", message,
            condition, function, file, line);
    std::terminate();
}
}  // namespace detail
}  // namespace ul
