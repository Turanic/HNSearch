#pragma once

#include <charconv>

namespace tools
{
  /* custom string -> numerics conversion function.
   * from_chars should be the standard conversion function in C++17.
   * However it is still quite low level and this wrapper will check
   * for errors permit one line numerics conversions.. */
  template <typename T>
  T string_to(std::string_view pattern)
  {
    T res{};
    auto err =
      std::from_chars(pattern.data(), pattern.data() + pattern.size(), res);

    if (err.ptr != pattern.data() + pattern.size())
      throw std::runtime_error{std::string{"string conversion failed"}};

    return res;
  }
} // namespace tools
