#pragma once

#include <string_view>
#include <vector>

namespace options
{
  class CmdLine
  {
  private:
    std::vector<std::string_view> args_{};

  public:
    CmdLine(int argc, char** argv);

    std::string_view at(std::size_t idx);

    using const_iterator = decltype(args_)::const_iterator;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_iterator erase(const_iterator it);
    std::size_t size() const noexcept;
  };
} // namespace options
