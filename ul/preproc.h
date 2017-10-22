#pragma once

// stringize: from boost 1.59
#ifdef _MSC_VER
#define UL_STRINGIZE(text) UL_STRINGIZE_A((text))
#define UL_STRINGIZE_A(arg) UL_STRINGIZE_I arg
#else
#define UL_STRINGIZE(text) UL_STRINGIZE_I(text)
#endif

#define UL_STRINGIZE_I(text) #text

// from boost 1.59
#define UL_CAT(a, b) UL_CAT_I(a, b)

#ifndef _MSC_VER
#define UL_CAT_I(a, b) a##b
#else
#define UL_CAT_I(a, b) UL_CAT_II(~, a##b)
#define UL_CAT_II(p, res) res
#endif
