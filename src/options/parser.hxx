#pragma once

namespace options
{
  template <typename... Args>
  OptionsParser::OptionsParser(Args&&... args)
      : cmd_{std::forward<Args>(args)...}
  {
  }

  template <typename Arg>
  Option* OptionsParser::add_option(Arg&& arg)
  {
    return options_
      .emplace_back(std::make_unique<Option>(std::forward<Arg>(arg)))
      .get();
  }
} // namespace options
