#pragma once

#include <optional>

namespace ul {

template <class T>
using maybe = std::optional<T>;
using nothing_t = std::nullopt_t;
inline constexpr auto nothing = std::nullopt;

}  // namespace ul
