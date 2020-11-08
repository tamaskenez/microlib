#pragma once

#include "ul/span.h"
#include "ul/string_par.h"

namespace ul {
  bool startswith(string_par s, string_par prefix);
  bool startswith(string_par s, char prefix);
  bool endswith(string_par s, string_par prefix);
  span<const char> trim(span<const char> s);
  void split(span<const char> s,
             const char* separators,
             std::vector<span<const char>>& xs);
  inline std::vector<span<const char>> split(span<const char> s,
                                             const char* separators)
  {
      std::vector<span<const char>> xs;
      split(s, separators, xs);
      return xs;
}
}  // namespace ul
