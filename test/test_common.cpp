#pragma once

#ifdef ULTEST_USING_STD
using namespace std;
#define STDCC
#else
#define STDCC std::
#endif

#ifdef ULTEST_USING_UL
using namespace ul;
#define ULCC
#else
#define ULCC ul::
#endif
