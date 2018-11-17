#pragma once

namespace options
{
  enum class ValPos
  {
    after,
    before,
    last
  };

  struct Value
  {
    std::string_view* value = nullptr;
    ValPos position = ValPos::after;
  };
} // namespace options
