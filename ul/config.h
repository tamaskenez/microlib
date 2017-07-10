#pragma once
// Abstraction of platform or compiler dependent constructs
//
// C++ standard
//
// - UL_HAVE_CPP11 and UL_HAVE_CPP14 indicates full c++11/14 support
// - UL_NO_CPP11_CONSTEXPR indicates lack of C++11 constexpr support
// - UL_NO_REF_QUALIFIERS indicates lack of C++11 ref-qualifiers support
// - UL_CONSTEXPR_IF_SUPPORTED evalutes to `constexpr` or empty string
//   depending on compiler support
// - UL_CONSTEXPR14 evaluates to `constexpr` if c++14 `constexpr` is supported
//   or to empty string if not
//
// Attributes
//
// - use UL_PRINTFLIKE(M, N) to mark a function as printf-like, that is,
//   having a format string as the Mth arg (numbering starts from 1) and
//   varags starting at Nth.
//   Use UL_PRINTFLIKE(M, 0) for va_list-style printf-like functions.
//   Increment M and N by one if used on member functions (1st argument is
//   `this`)

#include <cstdio>

#if defined __printflike
#define UL_PRINTFLIKE(M, N) __printflike(M, N)
#elif defined __GNUC__
#define UL_PRINTFLIKE(M, N) __attribute__((format(printf, M, N)))
#else
#define UL_PRINTFLIKE(M, N)
#endif

#if __cplusplus > 201100
#define UL_HAVE_CPP11  // full c++11
#endif

#if __cplusplus > 201400
#define UL_HAVE_CPP14  // full c++14
#endif

#if __cplusplus > 201700
#define UL_HAVE_CPP17  // full c++17
#endif

#if !defined UL_HAVE_CPP11 && _MSC_VER < 1900
#define UL_NO_CPP11_CONSTEXPR  // constexpr c++11
#define UL_NO_REF_QUALIFIERS
#endif

#ifdef UL_NO_CPP11_CONSTEXPR
#define UL_CONSTEXPR_IF_SUPPORTED
#else
#define UL_CONSTEXPR_IF_SUPPORTED constexpr
#endif

#ifdef UL_HAVE_CPP14
#define UL_CONSTEXPR14 constexpr
#else
#define UL_CONSTEXPR14
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#define UL_LIKELY(x) (__builtin_expect(!!(x), 1))
#define UL_UNLIKELY(x) (__builtin_expect(!!(x), 0))
#else
#define UL_LIKELY(x) (x)
#define UL_UNLIKELY(x) (x)
#endif

#ifdef _MSC_VER
#define UL_NORETURN __declspec(noreturn)
#elif defined UL_HAVE_CPP11
#define UL_NORETURN [[noreturn]]
#else
#define UL_NORETURN
#endif

#ifdef UL_HAVE_CPP14

#define UL_DEPRECATED [[deprecated]]
#ifdef UL_HAVE_CPP17
#define UL_DEPRECATED_REASON(X) [[deprecated(X)]]
#else
#define UL_DEPRECATED_REASON(X) [[deprecated]]
#endif  // UL_HAVE_CPP17

#elif defined __GNUC__

#define UL_DEPRECATED __attribute__((deprecated))
#define UL_DEPRECATED_REASON(X) __attribute__((deprecated(X)))

#elif defined(_MSC_VER)

#define UL_DEPRECATED __declspec(deprecated)
#define UL_DEPRECATED_REASON(X) __declspec(deprecated(X))

#else

#pragma message("WARNING: Please, implement UL_DEPRECATED for this compiler")
#define UL_DEPRECATED
#define UL_DEPRECATED_REASON

#endif

#ifdef UL_HAVE_CPP17
#define UL_NODISCARD [[nodiscard]]
#elif defined __GNUC__
#define UL_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#define UL_NODISCARD _Check_return_
#else
#pragma message("WARNING: Please, implement UL_NODISCARD for this compiler")
#define UL_NODISCARD
#endif

#ifdef UL_HAVE_CPP11
#define UL_NOEXCEPT noexcept
#else
#define UL_NOEXCEPT
#endif
