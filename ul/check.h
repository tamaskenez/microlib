#pragma once

// Synopsis
// --------
//
// Check condition:
//
//     CHECK(cond)
//
// Check condition with printf-like formatted message:
//
//     CHECK(cond, fmt_string[, args...])
//
// Set behaviour on failed checks:
//
//     set_check_failed_policy(
//         check_failed_policy_terminate | check_failed_policy_terminate)
//
// The `CHECK` macro may collide with the application's own `CHECK`.
// In that case define `UL_DONT_DEFINE_PLAIN_CHECK` and use `UL_CHECK` instead:
//
//     #define UL_DONT_DEFINE_PLAIN_CHECK
//     #include "adasworks/sx/check.h"
//     ...
//     UL_CHECK(cond)
//
// The `UL_CHECK` macro is always available.
//
//

#include "ul/config.h"

#if defined _MSC_VER
#define UL_FUNCTION __FUNCSIG__
#else
#define UL_FUNCTION __PRETTY_FUNCTION__
#endif

namespace ul {
namespace detail {

UL_NORETURN void check_failed_core(const char* condition,
                                   const char* file,
                                   int line,
                                   const char* function);

UL_NORETURN void check_failed_core(const char* condition,
                                   const char* file,
                                   int line,
                                   const char* function,
                                   const char* format_string,
                                   ...) UL_PRINTFLIKE(5, 6);
}
}

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

// Simple but non-standard implementation
#define UL_CHECK(condition, ...)                                            \
    ((UL_LIKELY(condition)) ? (void)0 : (::ul::detail::check_failed_core(   \
                                            #condition, __FILE__, __LINE__, \
                                            UL_FUNCTION, ##__VA_ARGS__)))

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#ifndef UL_DONT_DEFINE_PLAIN_CHECK
#define CHECK UL_CHECK
#endif
