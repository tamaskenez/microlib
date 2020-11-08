#pragma once

#include <string>

#include "ul/config.h"

namespace ul {

std::string stringf(const char* fmt, ...) UL_PRINTFLIKE(1, 2);
std::string vstringf(const char* fmt, va_list arg) UL_PRINTFLIKE(1, 0);
}  // namespace ul
