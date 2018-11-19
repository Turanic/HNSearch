#pragma once

namespace options
{
  /* Position of the value relatively to the option */
  enum class ValPos
  {
    after,
    before,
    last
  };

  /* Value associated with an option */
  struct Value
  {
    std::string_view* value = nullptr;
    ValPos position = ValPos::after;
  };
} // namespace options
